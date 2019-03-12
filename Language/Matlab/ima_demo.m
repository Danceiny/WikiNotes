% MATLAB???????

% ????????????????
imaqhwinfo

% ??videoinput??
obj = videoinput('winvideo');

% ????
set(obj, 'FramesPerTrigger', 1);
set(obj, 'TriggerRepeat', Inf);

% preview(obj);

% ????
hf = figure('Units', 'Normalized', 'Menubar', 'None', ...
    'NumberTitle', 'off', 'Name', 'danceiny:Show&Take photo');
ha = axes('Parent', hf, 'Units', 'Normalized', ...
    'Position', [.05 .2 .85 .7]);
axis off
hb1 = uicontrol('Parent', hf, 'Units', 'Normalized', ...
    'Position', [.25 .05 .2 .1], 'String', 'danceiny:preview', ...
    'Callback', ...
    ['objRes = get(obj, ''VideoResolution'');' ...
     'nBands = get(obj, ''NumberOfBands'');' ...
     'hImage = image(zeros(objRes(2), objRes(1), nBands));' ...
     'preview(obj, hImage);']);
hb2 = uicontrol('Parent', hf, 'Units', 'Normalized', ...
    'Position', [.55 .05 .2 .1], 'String', 'danceiny:take', ...
    'Callback', 'imwrite(getsnapshot(obj), ''im.jpg'')');

set(obj,'TimerPeriod',10)
start(obj)


