% Run with: `matlab -nodesktop -nosplash` for more ease

% Perform `addpath` on mex file's location
% @note You must ensure that it is built, and that you've sourced the appropriate
% `devel/setup.bash`!
name = 'matlab_interface_test';
[~, lib] = system(sprintf('catkin_find --first-only %s.%s', name, mexext()));
lib_path = fileparts(lib);
addpath(lib_path);

matlab_interface_test('help');
