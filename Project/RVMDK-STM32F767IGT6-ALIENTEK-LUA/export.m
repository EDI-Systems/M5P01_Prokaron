% [c,s] = txt2cell(str,delim)
b=hex2dec(raw);
c=[];
for i=1:480
    for j=1:272
        c(i,j)=b((i-1)*272+j);
    end
end

imshow(c,[]);

imgR = uint8((255/31).*bitshift(bitand(c, 63488), -11));  % Red component
imgG = uint8((255/63).*bitshift(bitand(c, 2016), -5));    % Green component
imgB = uint8((255/31).*bitand(c, 31));                    % Blue component

imgRGB = cat(3, imgR, imgG, imgB);  % Concatenate along the third dimension
imwrite(imgRGB, 'myImage.bmp');     % Output the image to a file