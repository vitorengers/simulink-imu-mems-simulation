function y = fcn(u)

global temp;
if temp == 0
   temp = u; 
   y = 0;
else
    y = u - temp;
end