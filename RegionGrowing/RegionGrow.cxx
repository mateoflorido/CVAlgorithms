#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <queue>
#include <vector>

using namespace cv;
const Point neighbors[8] = {Point(1,0),Point(1,-1),Point(-1,-1),Point(-1,0),Point(-1,1),Point(0,1),Point(0,-1),Point(1,1)};


float average(Point aux, Mat &image_gray){
    float averVal = 0;
    Scalar inten=image_gray.at<uchar>(aux.y, aux.x);
    int nNeigh = 0;
    for(int i =0; i<8 ; i++){
        Point actualPoint = aux + neighbors[i];
        if(actualPoint.x < 0 || actualPoint.y <0 || actualPoint.x > image_gray.size().width-1 || actualPoint.y > image_gray.size().height-1)
            continue;
        else{
            nNeigh++;
            inten=image_gray.at<uchar>(actualPoint.y, actualPoint.x);
            averVal+=inten.val[0];
        }
    }
    if(nNeigh!=0)
        return averVal/nNeigh;
    return 0;
}

Point seedGenerator(Mat &image) {
    Mat image_ycc, channels[3];
    std::vector<float> varImage;
    std::vector<Point> imagePoint;
    float variance;
    cvtColor(image, image_ycc, COLOR_RGB2YCrCb);
    split(image_ycc, channels);

}
float variance(Point aux, Mat &channel){
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
        if(verified[aux.y][aux.x]){
            active_front.pop();
            continue;
        }
        region.push_back(aux);
        for(int i =0; i<8 ;i++){
            Point actualPoint = aux + neighbors[i];
            //Check image limits
            if(actualPoint.x < 0 || actualPoint.y <0 || actualPoint.x > image_gray.size().width-1 || actualPoint.y > image_gray.size().height-1){

                continue;
            }

            else{
                if(std::abs((average(actualPoint, image_gray)-average(aux, image_gray)))<=Thresh){
                    active_front.push(actualPoint);
                }
            }
        }
        verified[aux.y][aux.x] = true;
        active_front.pop();
    }
    std::cout<<"Grow -> Después del while"<< std::endl;
    return region;

}
int main(int argc, char** argv )
{
    // Get command line arguments
    if ( argc < 5 )
    {
        std::cerr << "Usage: " << argv[ 0 ] << " image_file " <<  " x_seed_coordinates/-1 (-1 for automatic seed) " <<  " y_seed_coordinates/-1 (-1 for automatic seed) " << " umbral "<<std::endl;
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

    //Saving Thresh (umbral)
    int umbral = std::atoi(argv[4]);

    if ( !image.data )
    {
        std::cerr << "Error: No image data" << std::endl;
        return( -1);

    } // fi

    //Turn gray scale
    Mat image_gray;
    cvtColor(image, image_gray, COLOR_RGB2GRAY);


    //Setting seed point
    Point seed;
    seed = Point(std::atoi(argv[2]), std::atoi(argv[3]));
    if(seed.x == -1 || seed.y == -1){
        if(seed.y >0 || seed.x > 0){
            std::cerr<< "Invalid seed, for automatic seed both need to be -1 ..."<<std::endl;
        }
        std::cout<< "Automatic seed enabled..."<< std::endl;
        seed = seedGenerator(image_gray);
    } else {
        if(seed.x > image.size().width-1 || seed.x > image.size().height-1){
            std::cerr << "Seed point is outside image limits ..." <<std::endl;
        }
    }


   /* imshow("asdfs", image_gray);
    waitKey();*/

    //Growing
    std::cout<<"Va a entrar"<< std::endl;
    vector<Point> region = Grow(image_gray, seed, umbral);
    std::cout<<"Va a salir"<< std::endl;

    //Fill object
    Mat segmented = Mat::zeros(image.size(), CV_8UC3);
    cvtColor(segmented, segmented, COLOR_RGB2GRAY);


    std::cout<<region.size()<<std::endl;
    for(int i = 0; i<region.size();i++){
        //std::cout<<"Punto a agregar x="<<region[i].x<< " y="<<region[i].x<<std::endl;
        if(region[i].x==10 && region[i].y== 10){
            std::cout<<"Si el resultado es extraño, se recomienda elegir un umbral menor..."<<std::endl;
        }
        segmented.at<uchar>(region[i].y,region[i].x ) = 255;
    }

    //Write something
    std::stringstream ss ( argv[ 1 ] );
    std::string basename;
    getline( ss, basename, '.' );
    imwrite( basename + "_seg.png", segmented);
    return( 0 );
}

// eof - RegionGrow.cxx