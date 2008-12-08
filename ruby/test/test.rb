# Add to load path
$LOAD_PATH.unshift(File.expand_path(File.dirname(__FILE__) + '/../lib'))

# Load lunkwill
require 'lunkwill'

# Test
arg = Lunkwill::LWArgumentCreateFromString('Blah')
