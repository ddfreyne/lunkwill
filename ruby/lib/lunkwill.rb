# Load what's necessary
begin ; require 'rubygems' ; rescue LoadError ; end
require 'dl/import'
require 'dl/struct'

# Define Lunkwill
module Lunkwill

  # Main library details
  extend DL::Importable
  puts File.expand_path(File.dirname(__FILE__) + '/../../lunkwill.dylib')
  dlload File.expand_path(File.dirname(__FILE__) + '/../../lunkwill.dylib')

  # Types
  typealias('size_t',   'unsigned int')
  typealias('uint8_t',  'unsigned char')
  typealias('bool',     'unsigned char')

  # Structs
  LWArgument = struct [
    'size_t   length',
    'uint8_t  *data',
    'bool     isRetainable'
  ]

  # Functions
  extern 'LWArgument *LWArgumentCreateFromString(char *aString)'

end
