% Run with: `matlab -nodesktop -nosplash` for more ease

% Perform `addpath` on mex file's location
% @note You must ensure that it is built, and that you've sourced the appropriate
% `devel/setup.bash`!
name = 'matlab_interface_test';
[status, lib] = system(sprintf('catkin_find --first-only %s.%s', name, mexext()));
assert(status == 0, 'Could not find mex library: %s', name);
lib_path = fileparts(lib);
addpath(lib_path);

matlab_interface_test('help');

% Test variable arguments
[a, b, c, d, e] = matlab_interface_test('variable_arguments', 'something', {}, 'else');
x = [a, b, c, d, e];
x_expected = 1:5;
assert(all(x == x_expected));

% Test persistence
matlab_interface_test('reset_counter');
count = 5;
counters = zeros(1, count);
for i = 1:count
    counters(i) = matlab_interface_test('increment_counter');
end
counters_expected = 1:count;
assert(all(counters == counters_expected));

% Test creating structures
count = 5;
A = rand(count);
b = rand(1);
value_expected = -b * A';
name = 'Test';
obj_expected = struct('name', name, ...
    'cell', {{A, b, 'Hello'}});

% Use matlab_utilities
[~, pkg] = system('rospack find matlab_utilities');
pkg = strtrim(pkg);
addpath(fullfile(pkg, 'matlab'));
matlab_utilities_depends('general', 'yaml');
yaml_cmp = @(a, b) strcmp(yaml_dump(a), yaml_dump(b));
maxabs = @(x) max(abs(x(:)));

[obj, value] = matlab_interface_test('generate_struct', name, A, b);
assert(yaml_cmp(obj, obj_expected));
tol = 1e-12;
assert(maxabs(value - value_expected) < tol);

fprintf('All tests passed\n');
