hitMiss = importdata('/home/rghunter/OpticalQuad/OpticalFlowCV/data/hitMissEval.csv');
HitRate = hitMiss.data(:,1)/hitMiss.data(:,4);
FalseDetect = hitMiss.data(:,2)/hitMiss.data(:,4);
MissRate = (hitMiss.data(:,4)-(hitMiss.data(:,1)+hitMiss.data(:,2)))/hitMiss.data(:,4); 
outputData = [MissRate(:,1),HitRate(:,1),FalseDetect(:,1)];
%subplot(2,1,1)
plot(outputData)
legend('miss','hit','false')
%subplot(2,1,2)
figure(2)
scatter(FalseDetect(:,1),HitRate(:,1))
axis([0 1 0 1])
line = refline(1,0)
set(line,'Color','r')


%hold off;
%plot(HitRate,'g')
%hold on;
%plot(MissRate,'b')
%plot(FalseDetect,'r')

