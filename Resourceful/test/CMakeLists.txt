# - Tests for Resourceful project

# - Basic run
add_test(NAME testBasicExecutable COMMAND rsf_simple)

# - Self location
add_test(NAME testSelfLocation COMMAND rsf_simple)
set_property(TEST testSelfLocation PROPERTY PASS_REGULAR_EXPRESSION ".*br_find_exe = $<TARGET_FILE:rsf_simple>")

