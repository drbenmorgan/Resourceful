# - Tests for Resourceful project

# - Basic run
add_test(NAME testBasicExecutable COMMAND Resourceful)

# - Self location
add_test(NAME testSelfLocation COMMAND Resourceful)
set_property(TEST testSelfLocation PROPERTY PASS_REGULAR_EXPRESSION ".*getApplicationPath\\(\\) = $<TARGET_FILE_DIR:Resourceful>.*")


