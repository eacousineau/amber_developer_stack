%> @brief Basic ros unittests
close('all'); clear('all'); path(pathdef());

% Setup
[~, pkg] = system('rospack find matlab_utilities');
pkg = strtrim(pkg);
addpath(fullfile(pkg, 'matlab'));
matlab_utilities_depends('yaml');

%% Test
expected = struct();
expected.name = 'Billy';
expected.values = [1; 2; 3]; % Shape gets lost
str = yaml_dump(expected);

% Ensure that elements are converted to a matrix
actual = cell_to_matrix_scan(yaml_load(str));
assert(strcmp(expected.name, actual.name));
assert(all(expected.values == actual.values));

fprintf('All tests passed.\n');
