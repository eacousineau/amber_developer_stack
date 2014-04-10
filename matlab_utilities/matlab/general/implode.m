function [res] = implode(pieces, glue)
%implode  Stick together a cell array of strings using glue
%
% Example:
% >> implode({'a', 'b', 'c'}, '.')
% a.b.c

temp = repmat({glue}, length(pieces) * 2 - 1, 1);
temp(1:2:end) = pieces;
res = [temp{:}];

end