#include "SimpleImage.h"
#include <cmath>


// clamp a number
float clamp(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}


// find random number between 0 and 1
inline double findNoise(double x,double y)
{
  int n=(int)x+(int)y*57;
  n=(n<<13)^n;
  //int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
  int nn = (n*(n*n*15731+789221)+1376312589)&0x7fffffff; 

  return 1.0-((double)nn/1073741824.0);
}

// cosine interpolation
inline double interpolate(double a,double b,double x)
{
  double ft=x * 3.1415927;
  double f=(1.0-cos(ft))* 0.5;
  return a*(1.0-f)+b*f;
}

double SmoothNoise(double x, double y){
    double corners = ( findNoise(x-1, y-1)+findNoise(x+1, y-1)+findNoise(x-1, y+1)+findNoise(x+1, y+1) ) / 16.0;
    double sides   = ( findNoise(x-1, y)  +findNoise(x+1, y)  +findNoise(x, y-1)  +findNoise(x, y+1) ) /  8.0;
    double center  =  findNoise(x, y) / 4.0;

    return corners + sides + center;
}
    


// calculate noise for a given
double noise(double x,double y)
{
  double floorx=(double)((int)x);//This is kinda a cheap way to floor a double integer.
  double floory=(double)((int)y);


  double s,t,u,v;//Integer declaration
  s = SmoothNoise(floorx,floory); 
  t = SmoothNoise(floorx+1,floory);
  u = SmoothNoise(floorx,floory+1); //Get the surrounding pixels to calculate the transition.
  v = SmoothNoise(floorx+1,floory+1);

  double int1 = interpolate(s,t,x-floorx); //Interpolate between the values.
  double int2 = interpolate(u,v,x-floorx); //Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.

  return interpolate(int1,int2,y-floory);//Here we use y-floory, to get the 2nd dimension.
}




int main() {

  int octaves = 2;
  double zoom = 2.0;
  double persistance = 0.125;

  int w = 1000;
  int h = 1000;

  // gives the number of colors for the image
  int colors = 0;

  // Initialize noise_image image
  SimpleImage gradient(w, h, RGBColor(0,0,0));
  //SimpleImage noise(w, h, RGBColor(0,0,0));

  RGBColor firstCol = RGBColor(1,1,1);
  RGBColor secondCol = RGBColor(0.9,0.9,0.9);  
  //RGBColor firstCol = RGBColor(0,123/255.0,12/255.0); // green mountains
  // sand color // RGBColor secondCol = RGBColor(215/255.0,176/255.0,108/255.0);
  // green // RGBColor secondCol = RGBColor(118/255.0,155/255.0,0);
  RGBColor thirdCol = RGBColor(0,123/255.0,12/255.0);
  // RGBColor thirdCol = RGBColor(120/255.0,72/255.0,0);
  RGBColor moisture = RGBColor(0.7,0.7,0.7);



  // Iterate over pixels and set color for gradient and noisy image 
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      float py = y / (h*1.0);
      float px = x / (w*1.0);

      double getnoise = 0;
        
      //This loops trough the octaves.
      for(int i = 0 ; i < octaves; i++){

        double frequency = pow(2,i); //This increases the frequency with every loop of the octave.
        double amplitude = pow(persistance,i); //This decreases the amplitude with every loop of the octave.
    
        //This uses our perlin noise functions. It calculates all our zoom and frequency and amplitude
        getnoise += noise((double)x*frequency/zoom,(double)y/zoom*frequency)*amplitude;
      }  //  It gives a decimal value, you know, between the pixels. Like 4.2 or 5.1


      int color= (int)((getnoise*128.0)+128.0);//Convert to 0-256 values.
        
      if(color>255) color=255;
      if(color<0) color=0;


      float percent_noise = color/255.0;
      float R = 0.0;
      float G = 0.0;
      float B = 0.0;


      // change y by the cos of x to try to get waves
      px = px + clamp(0.05*cos(75*px)/2.0, -0.2, 0.2);
      //py = py + clamp(0.05*cos(60*py)/2.0, -0.2, 0.2);

       // The code here just generates white and black noise
      if (colors == 0){
        R =  (double)color ; 
        G =  (double)color ; 
        B =  (double)color ; 
      }

      else if (colors == 1){
        R =  clamp(firstCol.r *percent_noise /* + moisture.r * (1 - px) */ , 0.0 , 1.0);
        G =  clamp(firstCol.g *percent_noise /* + moisture.g * (1 - px) */ , 0.0, 1.0);
        B =  clamp(firstCol.b *percent_noise /* + moisture.b * (1 - px) */ , 0.0, 1.0);
      } 

      else if (colors ==2){
        R =  clamp(firstCol.r * (1 - py)*px + secondCol.r * py *(1-px ) * percent_noise /* + moisture.r */, 0.0, 1.0);
        G =  clamp(firstCol.g * (1 - py)*px + secondCol.g * py *(1-px ) * percent_noise /* + moisture.g */, 0.0, 1.0);
        B =  clamp(firstCol.b * (1 - py)*px + secondCol.b * py *(1-px ) * percent_noise /* + moisture.b */, 0.0, 1.0);
      }
      else if (colors == 3){

        //if(color < 100) color = 100;

        if (py < 0.5){
          R =  clamp((secondCol.r * (2*py) + firstCol.r * (0.5 - py) * 2.0  )*percent_noise /* + moisture.r * (1 - px) */, 0.0, 1.0);
          G =  clamp((secondCol.g * (2*py) + firstCol.g * (0.5 - py) * 2.0  )*percent_noise /* + moisture.g * (1 - px) */, 0.0, 1.0);
          B =  clamp((secondCol.b * (2*py) + firstCol.b * (0.5 - py) * 2.0  )*percent_noise /* + moisture.b * (1 - px) */, 0.0, 1.0);
        }  
        else { 
          R =  clamp((thirdCol.r * (py - 0.5) + secondCol.r * (1-py) * 2.0  )*percent_noise /* + moisture.r * (1 - px) */, 0.0, 1.0);
          G =  clamp((thirdCol.g * (py - 0.5) + secondCol.g * (1-py) * 2.0  )*percent_noise /* + moisture.g * (1 - px) */, 0.0, 1.0);
          B =  clamp((thirdCol.b * (py - 0.5) + secondCol.b * (1-py) * 2.0  )*percent_noise /* + moisture.b * (1 - px) */, 0.0, 1.0);        
        }
      }

      // RGBColor c = RGBColor(R,G,B);
      // noise_image.set(x, y, c);


      RGBColor c = RGBColor(R,G,B);
      gradient.set(x, y, c);

    }
  }

  // Save noise_image image to file
  //noise_image.save("noise.png");
  gradient.save("gradient.png");

  return 0;
}
