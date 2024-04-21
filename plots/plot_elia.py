import numpy as np
import matplotlib.pyplot as plt
import soundfile as sf


######## pitch evaluation #########

p = [x.split()[0] for x in open('/home/lmuixi/PAV/P3/pitch_db/train/rl002.f0').readlines()]
p = [float(x) for x in p]
p_ref = [x for x in open('/home/lmuixi/PAV/P3/pitch_db/train/rl002.f0ref').readlines()]
p_ref = [float(x) for x in p_ref]

t = np.arange(len(p))


######## autocorrelation #########

wavefile, f = sf.read('/home/lmuixi/PAV/P3/pitch_db/train/rl002.wav')
begin_segment = int(0.340 * f)
end_segment = begin_segment + int(0.03 * f)
duration = end_segment-begin_segment
wavefile_segment = wavefile[begin_segment:end_segment]

r = np.zeros(duration)


for k in range(duration):
   for n in range(len(wavefile_segment)-k):
     r[k] = r[k] + (wavefile_segment[n] * wavefile_segment[k+n])   
   r[k] = r[k] /len(wavefile_segment)


#### pitch ####
fig, ((plt1, plt2), (plt3, plt4)) = plt.subplots(2, 2)
plt1.plot(t,p,'-')
plt1.set_title('Pitch')
plt1.set_ylabel('[Hz]')

plt2.plot(t,p_ref,'-')
plt2.set_title('Pitch ref')
plt2.set_ylabel('[Hz]')

#### autocorrelation ###

plt3.plot(wavefile_segment)
plt3.set_title('Signal')
plt3.set_ylabel('[time]')

plt4.plot(r)
plt4.set_title('Autocorrelation')
plt4.set_ylabel('[samples]')

plt.show()