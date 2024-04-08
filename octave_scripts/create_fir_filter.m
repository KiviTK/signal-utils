pkg load signal
fid = fopen('coeffs','wt');

 coeffs = fir1(5, 0.002)
 if fid > 0
     fprintf(fid,'%d,',coeffs');
     fclose(fid);
 end
