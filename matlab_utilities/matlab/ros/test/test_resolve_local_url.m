%> @brief Basic unittests

actual = ros_resolve_local_url('package://roscpp');
expected = rospack_find('roscpp');
assert(strcmp(actual, expected));

actual = ros_resolve_local_url('package://roscpp/package.xml');
expected = fullfile(rospack_find('roscpp'), 'package.xml');
assert(strcmp(actual, expected));

fprintf('All tests passed.\n');
