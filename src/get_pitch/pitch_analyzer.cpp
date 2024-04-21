/// @file

#include <iostream>
#include <math.h>
#include "pitch_analyzer.h"

using namespace std;

FILE *r1 = fopen("r1.txt", "w+");
FILE *rmax = fopen("rmax.txt", "w+");  
FILE *potf = fopen("pot.txt", "w+");

/// Name space of UPC
namespace upc {
  void PitchAnalyzer::autocorrelation(const vector<float> &x, vector<float> &r) const {

    for (unsigned int l = 0; l < r.size(); ++l) {
  		/// \TODO Compute the autocorrelation r[l] 
      /// \DONE
      /// - inicio 
      /// - calculo
      /// - resultado 
      r[l]=0.0f;
      for(unsigned int n = l; n < x.size(); n++){
          r[l]+=x[n]*x[n-l];
      }
        
      r[l]=r[l]/x.size();
    }
    

    if (r[0] == 0.0F) //to avoid log() and divide zero 
      r[0] = 1e-10; 

    
  }

  void PitchAnalyzer::set_window(Window win_type) {
    if (frameLen == 0)
      return;

    window.resize(frameLen);

    switch (win_type) {
    case HAMMING:
      /// \TODO Implement the Hamming window
      for(unsigned int i = 0; i < frameLen; ++i)
        window[i] = 0.54- 0.46 * cos(2 * M_PI * i / (frameLen - 1));
      
      break;
    case RECT:
    default:
      window.assign(frameLen, 1);
    }
  }

  void PitchAnalyzer::set_f0_range(float min_F0, float max_F0) {
    npitch_min = (unsigned int) samplingFreq/max_F0;
    if (npitch_min < 2)
      npitch_min = 2;  // samplingFreq/2

    npitch_max = 1 + (unsigned int) samplingFreq/min_F0;

    //frameLen should include at least 2*T0
    if (npitch_max > frameLen/2)
      npitch_max = frameLen/2;
  }

  bool PitchAnalyzer::unvoiced(float pot, float r1norm, float rmaxnorm) const {
    /// \TODO Implement a rule to decide whether the sound is voiced or not.
    /// * You can use the standard features (pot, r1norm, rmaxnorm),
    ///   or compute and use other ones.
    if((rmaxnorm > 0.4 || r1norm > 1.5) && pot > -46 )
      return false;
    else
      return true;
  }

  float PitchAnalyzer::compute_pitch(vector<float> & x) const {
    if (x.size() != frameLen)
      return -1.0F;

    // center-clipping
    float max = *std::max_element(x.begin(), x.end());
    for(int i = 0; i < (int)x.size(); i++) {
      if(abs(x[i]) < 0.0048) {
        x[i] = 0.0F;
      }
    }
    //Frame normalization
    for (int i = 0; i < (int)x.size(); i++)
      x[i] /= max;

    //Window input frame
    for (unsigned int i=0; i<x.size(); ++i)
      x[i] *= window[i];

    vector<float> r(npitch_max);

    //Compute correlation
    autocorrelation(x, r);

    vector<float>::const_iterator iR = r.begin(), iRMax = iR+ npitch_min;
    //iRMax=r[1];
    //float max=r[1];
    //int pos;
    // buscar posicion maxima
    for(iR = r.begin()+npitch_min; iR< r.begin()+npitch_max;iR++){
        if(*iR>*iRMax){
          iRMax = iR;
        }
          
    }

    /// \TODO 
	/// Find the lag of the maximum value of the autocorrelation away from the origin.<br>
	/// Choices to set the minimum value of the lag are:
	///    - The first negative value of the autocorrelation.
	///    - The lag corresponding to the maximum value of the pitch.
    ///	   .
	/// In either case, the lag should not exceed that of the minimum value of the pitch.

    unsigned int lag = iRMax - r.begin();

    float pot = 10 * log10(r[0]);

    


    //You can print these (and other) features, look at them using wavesurfer
    //Based on that, implement a rule for unvoiced
    //change to #if 1 and compile
#if 1
    if (r[0] > 0.0F)
      cout << pot << '\t' << r[1]/r[0] << '\t' << r[lag]/r[0] << endl;
#endif
    
    if (unvoiced(pot, r[1]/r[0], r[lag]/r[0]))
      return 0;
    else{
      
      
        /* FILE *file_voz_x = fopen("voz_x.txt", "w+");
        FILE *file_voz_r = fopen("voz_r.txt", "w+");

        for (unsigned int i = 0; i < x.size(); i++)
        {
          fprintf(file_voz_x, "%f \n", x[i]);
          fprintf(file_voz_r, "%f \n", r[i]);
        }

        fclose(file_voz_x);
        fclose(file_voz_r);*/

      fprintf(r1 , "%f \n", r[1]/r[0]);
    fprintf(rmax , "%f \n", r[lag]/r[0]);
    fprintf(potf , "%f \n", pot);
        
       return (float) samplingFreq/(float) lag;
    }
     
  }
}