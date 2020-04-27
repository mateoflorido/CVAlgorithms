#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <queue>
#include <vector>

using namespace cv;
const Point neighbors[8] = {Point(1,0),Point(1,-1),Point(-1,-1),Point(-1,0),Point(-1,1),Point(0,1),Point(0,-1),Point(1,1)};
float average(Point &aux, Mat &image_gray){
  float averVal = 0;
  int nNeigh = 0;
  for(int i =0; i<8 ; i++){
    Point actualPoint = aux + neighbors[i];
    if(actualPoint.x < 0 || actualPoint.y <0 || actualPoint.x > image_gray.size().width-1 || actualPoint.y > image_gray.size().height-1)
      continue;
    else{
      nNeigh++;
      averVal+=image_gray.at<int>(actualPoint);
    }
  }
  nNeigh++;
  averVal+=image_gray.at<int>(aux);
  return averVal/nNeigh;
}

float variance(std::vector<int> points){
  /*float averageValue = average(points);
  float total=0;
  float minus=0;
  for(int i =0; i< points.size(); i++){
      minus=(float)points[i]-averageValue;
      total+=minus*minus;
  }
  return total/points.size();*/
  return 0.1;
}

int contrast(std::vector<int> points){
  /*int max=0;
  int min=0;
  max=*std::max_element(points.begin(),points.end());
  min=*std::min_element(points.begin(),points.end());
  return max-min;*/ 
  return 1;
}

std::vector<Point> Grow(Mat &image_gray, Point &seed, float Thresh){
  Vector<int> points;
  Point aux;
  vector<Point> region;
  vector<vector<bool>> verified;
  for(int i =0; i<image_gray.size().height ; i++){
    verified.push_back(std::vector<bool> (image_gray.size().width, false));
  }
  std::queue <Point> active_front;
  active_front.push(seed);
  image_gray.at<int>(seed.y, seed.x);
  //Growing algorithm
  std::cout<<"Grow -> Antes del while"<< std::endl;
  while(!active_front.empty()){
    
    aux=active_front.front();
    if(verified[aux.x][aux.y]){
      active_front.pop();
      continue;
    }
    region.push_back(aux);
    for(int i =0; i<8 ;i++){
      Point actualPoint = aux + neighbors[i];
      //Check image limits
      if(actualPoint.x < 0 || actualPoint.y <0 || actualPoint.x > image_gray.size().width-1 || actualPoint.y > image_gray.size().height-1)
        continue;
      else{
          if((average(actualPoint, image_gray)-average(aux, image_gray))<=Thresh){
            //std::cout<<"pushing x="<< actualPoint.x<<" y="<< actualPoint.y << std::endl;
              active_front.push(actualPoint);
          }
      }
    }
    verified[aux.x][aux.y] = true;
    active_front.pop();
  }
  std::cout<<"Grow -> Después del while"<< std::endl;
  return region;

}
int main(int argc, char** argv )
{
  // Get command line arguments
  if ( argc < 4 )
  {
  	std::cerr << "Usage: " << argv[ 0 ] << " image_file" << argv[ 1 ] << "x_seed_coordinates" << argv[ 2 ] << "y_seed_coordinates" <<std::endl;
    return( -1 );

  } // fi

  // Review given command line arguments
  std::cout << "-------------------------" << std::endl;
  for( int a = 0; a < argc; a++ )
    std::cout << argv[ a ] << std::endl;
  std::cout << "-------------------------" << std::endl;

  // Read an image
  Mat image;
  image = imread( argv[1], 1 );
  //PARA HACERLO AUTOMÁTICO SE PUEDEN ELEGIR LOS PUNTOS CON MAYOR INTENSIDAD DEL HISTOGRAMA
  Point seed= Point(std::atoi(argv[2]), std::atoi(argv[3]));

  if ( !image.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  
  } // fi

  //Turn gray scale
  Mat image_gray;
  cvtColor(image, image_gray, COLOR_RGB2GRAY);
  
  //Growing
  std::cout<<"Va a entrar"<< std::endl;
  vector<Point> region = Grow(image_gray, seed, 40);
  std::cout<<"Va a salir"<< std::endl;
  //Fill object
  Mat segmented = Mat::zeros(image.size(), CV_8UC3);
  std::cout<<"Number of channels in segmented before" << segmented.channels()<<std::endl;
  cvtColor(segmented, segmented, COLOR_RGB2GRAY);
  std::cout<<"Number of channels in segmented after" << segmented.channels()<<std::endl;
    
  //Write something
  std::stringstream ss ( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );
  imwrite( basename + "_seg.png", segmented);
  std::cout<<region.size()<<std::endl;
  for(int i = 0; i<region.size();i++){
    //std::cout<<"Punto a agregar x="<<region[i].x<< " y="<<region[i].x<<std::endl;
    segmented.at<uchar>(region[i]) = 255;
  }
  
  //Write something

  imwrite( basename + "_seg2.png", segmented);
  return( 0 );
}
// eof - RegionGrow.cxx