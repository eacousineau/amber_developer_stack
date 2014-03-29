%> @brief Catkin-style basic dependencies from matlab_utilities
%> @param vararigin List of subdirectories to add to the Matlab path
%> Each dependency must be a subdirectory under this one.
function [] = matlab_utilities_depends(varargin)

% Get path of this file
base_path = fileparts(mfilename('fullpath'));

for i = 1:nargin
    dep = varargin{i};
    
    % Check then add path
    dep_path = fullfile(base_path, dep);
    assert(isdir(dep_path), 'Invalid dependency (directory does not exist): %s', dep_path);
    addpath(dep_path);
    
    % Handle specifics
    switch dep
        case 'yaml'
            yaml_init();
        case 'spatial_v2'
            spatial_v2_init();
    end
end

end
