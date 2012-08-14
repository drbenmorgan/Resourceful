# - Tests for Resourceful project

# - Basic run
add_test(NAME testBasicExecutable COMMAND Resourceful)

# - Self location
# Not perfect, as uses the LOCATION target property which is somewhat
# deprecated and may not work with all generators (where the build mode
# may enter the config name).
add_test(NAME testSelfLocation COMMAND Resourceful)
get_property(_expectedlocation TARGET Resourceful PROPERTY LOCATION)
set_property(TEST testSelfLocation PROPERTY PASS_REGULAR_EXPRESSION "Located self at ${_expectedlocation}")

