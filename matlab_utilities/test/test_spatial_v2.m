%> @brief Basic spatial_v2 unittests
function [] = test_spatial_v2()

path(pathdef());

[~, pkg] = system('rospack find matlab_utilities');
pkg = strtrim(pkg);
addpath(fullfile(pkg, 'matlab'));
matlab_utilities_depends('spatial_v2');



end

