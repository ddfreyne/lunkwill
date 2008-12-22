# Lunkwill Howto

This document is a quick overview of how Lunkwill works. This document, just
like Lunkwill, is divided into four parts: Arguments, Messages, Data Handlers
and Validators.

## Arguments

Arguments are bits of data, created from integers, strings, or just raw
bytes. They are used in messages.

### Creating Arguments

To create an argument, use any of the following functions;

	LWArgument *LWArgumentCreate(void *aData, size_t aLength);
	LWArgument *LWArgumentCreateWithoutCopying(void *aData, size_t aLength);
	LWArgument *LWArgumentCreateFromString(char *aString);
	LWArgument *LWArgumentCreateFrom8BitInteger(int8_t aInteger);
	LWArgument *LWArgumentCreateFrom8BitUnsignedInteger(uint8_t aInteger);
	LWArgument *LWArgumentCreateFrom16BitInteger(int16_t aInteger);
	LWArgument *LWArgumentCreateFrom16BitUnsignedInteger(uint16_t aInteger);
	LWArgument *LWArgumentCreateFrom32BitInteger(int32_t aInteger);
	LWArgument *LWArgumentCreateFrom32BitUnsignedInteger(uint32_t aInteger);

The `LWArgumentCreate` function creates an argument by copying the given data
into an internal buffer. The `LWArgumentCreateWithoutCopying` function does
the same, but does not copy any data, so make sure the data you give to this
function stays alive while the argument is still around.

The other 7 functions create arguments from strings and integers of given
signedness and width. These functions all copy data to an internal buffer. The
16-bit and 32-bit (unsigned) integer functions will also convert integers to
network byte order.

For example:

	LWArgument *argument1 = LWArgumentCreateFromString("hello");
	LWArgument *argument2 = LWArgumentCreateFrom8BitInteger(-12);

### Deleting Arguments

To delete an argument, use `LWArgumentDelete`. This function looks like this:

	void LWArgumentDelete(LWArgument *aArgument);

For example:

	LWArgument *argument = LWArgumentCreateFromString("hello");
	LWArgumentDelete(argument);

Deleting an argument will also delete (i.e. `free`) the data associated with the argument, except if the argument was created without copying. To control an argument's ownership over its data, use `LWArgumentSetOwnsData`. Pass it `true` to ensure that `LWArgumentDelete` also deletes the data, or pass it `false` to tell `LWArgumentDelete` not to touch the data. This function looks like this:

	void LWArgumentSetOwnsData(LWArgument *aArgument, bool aOwnsData);

### Querying Arguments

To extract data from an argument, use the following functions:

	void    *LWArgumentGetData(LWArgument *aArgument);
	size_t   LWArgumentGetLength(LWArgument *aArgument);
	char    *LWArgumentGetStringValue(LWArgument *aArgument);
	int8_t   LWArgumentGet8BitIntegerValue(LWArgument *aArgument);
	uint8_t  LWArgumentGet8BitUnsignedIntegerValue(LWArgument *aArgument);
	int16_t  LWArgumentGet16BitIntegerValue(LWArgument *aArgument);
	uint16_t LWArgumentGet16BitUnsignedIntegerValue(LWArgument *aArgument);
	int32_t  LWArgumentGet32BitIntegerValue(LWArgument *aArgument);
	uint32_t LWArgumentGet32BitUnsignedIntegerValue(LWArgument *aArgument);

The first two functions return the raw data and the raw data length.

The string function returns a newly allocated, 0-terminated string with the
raw data interpreted as a string. Don't forget to free the C string to prevent
a memory leak.

The integer functions interpret the argument as an integer, perform any byte
swapping if necessary, and return the result. Note that the integer functions
expect the arguments to be of a specific length.

## Messages

A message is the most central object in Lunkwill. It consists of a one-byte
message ID, and an array of arguments. It's comparable to a function call: the
message ID identifies the function, and the function takes arguments.

### Creating Messages

To create a message, use either `LWMessageCreate` or `LWMessageCreate2`. These
functions look like this:

	LWMessage *LWMessageCreate(uint8_t aMessageID,
	    LWArgument *aArgument, ...);
	LWMessage *LWMessageCreate2(uint8_t aMessageID, size_t aArgumentCount,
	    LWArgument **aArguments);

The first function takes a message ID and a `NULL`-terminated argument list.
The latter takes an array of `LWArgument` pointers as well as the size of the
array. Both functions return a newly allocated message.

For example:

	LWMessage *message1 = LWMessageCreate(123, NULL);
	LWMessage *message2 = LWMessageCreate(123, someArgument,
	    anotherArgument, NULL);
	LWMessage *message3 = LWMessageCreate2(123, 2, arguments);

### Deleting Messages

Don't forget to delete messages after they have been used. Use the
`LWMessageDelete` function to do this. This function looks like this:

	void LWMessageDelete(LWMessage *aMessage);

This function will also delete any arguments used in the message, with the
exception of non-retainable arguments (see next section).

For example:

	LWMessage *message = LWMessageCreate(123, NULL);
	LWMessageDelete(message);

### Using Arguments In Multiple Messages

When a message is deleted, all its arguments are automatically deleted as
well. If you want to prevent an argument from being deleted when a message it
belongs to is deleted, make the argument non-retainable.

To do this, use the `LWArgumentSetRetainable` function, which looks like this:

	void LWArgumentSetRetainable(LWArgument *aArgument, bool aIsRetainable);

For example:

	LWArgument *argument1 = LWArgumentCreateFromString("hello");
	LWArgument *argument2 = LWArgumentCreateFromString("bye");
	LWMessage *message1 = LWMessageCreate(123, argument1, NULL);
	LWMessage *message2 = LWMessageCreate(234, argument1, argument2, NULL);
	LWArgumentSetRetainable(argument1, true);
	LWMessageDelete(message1); // argument1 not deleted
	LWMessageDelete(message2); // argument2 deleted
	LWArgumentDelete(argument1);

### Querying Messages

To extract data from a message, use the following functions:

	uint8_t     LWMessageGetMessageID(LWMessage *aMessage);
	size_t      LWMessageGetArgumentCount(LWMessage *aMessage);
	LWArgument *LWMessageGetArgumentAtIndex(LWMessage *aMessage,
	    size_t aIndex);

These functions return the message ID, the number of arguments in the message,
and the argument at the specific index of the message, respectively.

### Serializing Messages

To serialize a message, i.e. turn it into data that can be sent over the
network, use `LWMessageSerialize`. The resulting data does not need any
special treatment before it can be sent (integers are converted to network
byte order automatically, for example).

There is also an `LWMessageDeserialize` function, which converts data received
over the network back into a useable format. This is used internally by
`LWDataHandler`; you should generally not need to use it yourself.

## Data Handlers

A data handler is an object that collects data, attempts to extract as many
messages as possible, validate the messages, and finally call the appropriate
callback for a given message ID.

A data handler has callbacks for every message ID. These callbacks will be
called when a message with a matching message ID is received. Additionally,
the data handler has callbacks for unrecognised and invalid messages.

### Creating Data Handlers

Use the `LWDataHandlerCreate` function to create a data handler, which looks
like this:

	LWDataHandler *LWDataHandlerCreate(void *aUserInfo);

Its argument, `aUserInfo`, can be anything, including `NULL`; it will be
passed to any callbacks you specify, but will never be dereferenced.

For example:

	LWDataHandler *datahandler = LWDataHandlerCreate(NULL);

### Setting Data Handler Callbacks

There's three kinds of callbacks: "unrecognised message" callbacks, "invalid
message" callbacks, and normal "message" callbacks. "Unrecognised message"
callbacks are called whenever a message is received that does not have a
corresponding callback. "Invalid message" callbacks are called whenever a
invalid message is received (see below on how to use the validator). "Message"
callbacks are called when a recognised, valid message is received.

To set the callbacks, use the following functions:

	void LWDataHandlerSetUnrecognisedMessageCallback(
	    LWDataHandler *aDataHandler, LWDataHandlerCallback aCallback);
	void LWDataHandlerSetInvalidMessageCallback(
	    LWDataHandler *aDataHandler, LWDataHandlerCallback aCallback);
	void LWDataHandlerSetMessageCallback(
	    LWDataHandler *aDataHandler, uint8_t aMessageID,
	    LWDataHandlerCallback aCallback);

Also possibly useful is the `LWDataHandlerClearMessageCallbacks` function,
which clears all message callbacks. This can be used when switching between
protocols, for example. This function looks like this:

	void LWDataHandlerClearMessageCallbacks(LWDataHandler *aDataHandler);

A callback is a function with the prototype

	void my_callback(LWDataHandler *aDataHandler, LWMessage *aMessage,
	    void *aUserInfo)

For example:

	void message_123_callback(LWDataHandler *aDataHandler,
	    LWMessage *aMessage, void *aUserInfo)
	{
		// ...
	}

	int main(void)
	{
		// ...

		// Create data handler
		LWDataHandler *dataHandler = LWDataHandlerCreate(NULL);

		// Set a callback
		LWDataHandlerSetMessageCallback(dataHandler, 123,
		    &message_123_callback);

		// ...
	}

### Handling Data

When you have received data, simply pass it on to the data handler, using the
`LWDataHandlerHandleData` function, which looks like this:

	bool LWDataHandlerHandleData(LWDataHandler *aDataHandler, void *aData,
	    size_t aDataLength);

The data handler will then parse the data into messages, validate the messages
(see below), and call the appropriate callbacks.

For example:

	uint8_t buffer[1000];
	size_t bytes_received;

	bytes_received = recv(socket, buffer, 1000, 0);

	LWDataHandlerHandleData(dataHandler, buffer, bytes_received);

## Validators

A validator is a structure that determines whether a given message is valid
or not. It is usually used in conjunction with a data handler.

### Creating Validators

Use the `LWValidatorCreate` function to create a validator, which looks like
this:

	LWValidator *LWValidatorCreate(void);

### Deleting Validators

A validator can be deleted using the `LWValidatorDelete` function, which looks
like this:

    void LWValidatorDelete(LWValidator *aValidator);

### Setting Validation Rules

To set a validation callback for a specific message, use the
`LWValidatorSetValidationCallback` function, which looks like this:

    void LWValidatorSetMessageValidationCallback(LWValidator *aValidator,
        uint8_t aMessageID, LWValidatorMessageValidationCallback aCallback);

A validation callback is a function returning a boolean, and taking only one
argument, a message. For example:

    bool sample_message_validator(LWMessage *aMessage)
    {
        // This message does not have any arguments
        if(LWMessageGetArgumentsCount(aMessage) > 0)
            return false;
        else
            return true;
    }

You can also clear all message validator callbacks using the
`LWValidatorClearMessageValidationCallbacks` function, like this:

    LWValidatorClearMessageValidationCallbacks(validator);

### Validation Individual Messages

You don't usually need to manually validate individual messages (use the data
handler instead), but it may come in handy at times.

To manually validate a specific message, use the `LWValidatorValidateMessage`
function, which looks like this:

    bool LWValidatorValidateMessage(LWValidator *aValidator,
        LWMessage *aMessage);

This function returns true or false depending on whether the message is valid
or not. Messages without matching message validator callback are assumed to be
valid.

For example:

    bool isValid = LWValidatorValidateMessage(validator, message);

### Using Validators in Conjunction with Data Handlers

To let a data handler use a validator, use the `LWDataHandlerSetValidator`
function, which looks like this:

    void LWDataHandlerSetValidator(LWDataHandler *aDataHandler,
        LWValidator *aValidator);

For example:

    LWDataHandlerSetValidator(dataHandler, validator);
