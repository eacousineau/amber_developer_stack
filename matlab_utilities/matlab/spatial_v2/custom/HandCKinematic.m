%> @brief HandCKinematic Extension to HandC to return kinematics as well
%> @note From amber.control.HandCKinematic
function  [H, C, kin] = HandCKinematic(model, q, qd, kinematicOnly)

% TODO Get page references to these things, or look at RBDL
% Put in original software package, docs, license, etc
% ID p.96

% Is there a way to sneak jacobians in here? Or extract it from constrained
% dynamics?

% Using similar structure to RBDL

if nargin < 4
    kinematicOnly = false;
end

NB = model.NB;

blank = cell(NB, 1);
S = blank;
XJs = blank;
Xup = blank; % What does 'up' mean?!
Xbase = blank; % base to body frame
fvp = blank;
v = blank;
avp = blank; % Why avp? Spatial acceleration? How to calculate classic in base frame?????????????????
% ???????????????????

a_grav = get_gravity(model);

% Compute Kinematics
for i = 1:model.NB
    [ XJ, S{i} ] = jcalc( model.jtype{i}, q(i) );
    vJ = S{i} * qd(i);
    % Body frame effect (XJ) orienting the base transform (Xtree)
    % Transforms parent frame -> current
    Xup{i} = XJ * model.Xtree{i};
    
    XJs{i} = XJ;
    
    lambda = model.parent(i);
    if lambda == 0
        v{i} = vJ;
        avp{i} = Xup{i} * -a_grav; % gravity, parent -> reference? link, or just first?
        Xbase{i} = Xup{i};
    else
        v{i} = Xup{i} * v{lambda} + vJ;
        avp{i} = Xup{i} * avp{lambda} + crm(v{i}) * vJ;
        Xbase{i} = Xup{i} * Xbase{lambda};
    end
    fvp{i} = model.I{i} * avp{i} + crf(v{i}) * model.I{i} * v{i};
end

if nargout > 2
    kin = struct();
    kin.S = S;
    kin.XJ = XJs;
    kin.Xup = Xup;
    kin.Xbase = Xbase;
    kin.v = v;
end

if kinematicOnly
    H = [];
    C = [];
    return;
end

% Compute Coriolis, C = C_e(q, dq) dq + G(q)
for i = model.NB:-1:1
    C(i,1) = S{i}' * fvp{i};
    lambda = model.parent(i);
    if lambda ~= 0
        % Transpose is property of adjoint: X* = inv(X)'  ==>  inv(X*) = X'
        fvp{lambda} = fvp{lambda} + Xup{i}' * fvp{i};
    end
end

% Compute Inertia
IC = model.I;				% composite inertia calculation

for i = model.NB:-1:1
    % Look up reason for this
    lambda = model.parent(i);
    if lambda ~= 0
        % Backwards propagate
        IC{lambda} = IC{lambda} + Xup{i}' * IC{i} * Xup{i};
    end
end

H = zeros(model.NB);

% TODO Sensitivity for inertial parameters... Painful with cross products?
for i = 1:model.NB
    % Look up what this is analogous to... J' * I J ??? Looks like it
    % All in body frame? Seems so
    fh = IC{i} * S{i};
    H(i,i) = S{i}' * fh;
    j = i;
    while model.parent(j) > 0
        fh = Xup{j}' * fh;
        j = model.parent(j);
        H(i,j) = S{j}' * fh;
        H(j,i) = H(i,j);
    end
end

end
