#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <queue>
#include <vector>

using namespace cv;
const Point neighbors[8] = {Point(1, 0), Point(1, -1), Point(-1, -1), Point(-1, 0), Point(-1, 1), Point(0, 1),
                            Point(0, -1), Point(1, 1)};


float average(Point aux, Mat &image_gray, int numNeighbors) {
    float averVal = 0;
    Scalar inten = image_gray.at<uchar>(aux.y, aux.x);
    int nNeigh = 0;
    for (int i = 0; i < 8; i++) {
        if(numNeighbors == 4){
            if(neighbors[i].x == -1 && neighbors[i].y == -1)
                continue;
            if(neighbors[i].x == 1 && neighbors[i].y == -1)
                continue;
            if(neighbors[i].x == -1 && neighbors[i].y == 1)
                continue;
            if(neighbors[i].x == 1 && neighbors[i].y == 1)
                continue;
        }
        Point actualPoint = aux + neighbors[i];
        if (actualPoint.x < 0 || actualPoint.y < 0 || actualPoint.x > image_gray.size().width - 1 ||
            actualPoint.y > image_gray.size().height - 1)
            continue;
        else {
            nNeigh++;
            inten = image_gray.at<uchar>(actualPoint.y, actualPoint.x);
            averVal += inten.val[0];
        }
    }
    if (nNeigh != 0)
        return averVal / nNeigh;
    return 0;
}

Point seedGenerator(Mat &image) {
    Mat histogram;
    Point minPoint;
    Point maxPoint;
    double min, max;
    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange {range};
    bool uniform = true;
    bool accumulate = false;
    calcHist(&image,1,0,Mat(), histogram,1,&histSize,&histRange,uniform,accumulate);
    cv::minMaxLoc(histogram, &min, &max, &minPoint, &maxPoint);
    int i,j;
    uchar* p;
    for( i = 0; i < image.rows; ++i)
    {
        p = image.ptr<uchar>(i);
        for ( j = 0; j < image.cols; ++j)
        {
            if(p[j] == maxPoint.x)
            {
                return Point(i, j);
            }
        }
    }
    return maxPoint;
}

float variance(Point aux, Mat &image_gray , int numNeighbors) {
    float auxVal = 0;
    float averVal = average(aux, image_gray, numNeighbors);
    Scalar inten = image_gray.at<uchar>(aux.y, aux.x);
    int nNeigh = 0;
    for (int i = 0; i < 8; i++) {
        if(numNeighbors == 4){
            if(neighbors[i].x == -1 && neighbors[i].y == -1)
                continue;
            if(neighbors[i].x == 1 && neighbors[i].y == -1)
                continue;
            if(neighbors[i].x == -1 && neighbors[i].y == 1)
                continue;
            if(neighbors[i].x == 1 && neighbors[i].y == 1)
                continue;
        }
        Point actualPoint = aux + neighbors[i];
        if (actualPoint.x < 0 || actualPoint.y < 0 || actualPoint.x > image_gray.size().width - 1 ||
            actualPoint.y > image_gray.size().height - 1)
            continue;
        else {
            nNeigh++;
            inten = image_gray.at<uchar>(actualPoint.y, actualPoint.x);
            auxVal+= (inten.val[0]-averVal)*(inten.val[0]-averVal);
        }
    }
    if (nNeigh != 0)
        return auxVal/nNeigh;
    return 0;
}

int contrast(Point aux, Mat &image_gray, int numNeighbors) {
    int max = -1;
    int min = 256;
    Scalar inten;
    for (int i = 0; i < 8; i++) {
        if(numNeighbors == 4){
            if(neighbors[i].x == -1 && neighbors[i].y == -1)
                continue;
            if(neighbors[i].x == 1 && neighbors[i].y == -1)
                continue;
            if(neighbors[i].x == -1 && neighbors[i].y == 1)
                continue;
            if(neighbors[i].x == 1 && neighbors[i].y == 1)
                continue;
        }
        Point actualPoint = aux + neighbors[i];
        if (actualPoint.x < 0 || actualPoint.y < 0 || actualPoint.x > image_gray.size().width - 1 ||
            actualPoint.y > image_gray.size().height - 1)
            continue;
        else {
            inten = image_gray.at<uchar>(actualPoint.y, actualPoint.x);
            if(inten.val[0]>max)
                max = inten.val[0];
            if(inten.val[0]<min)
                min = inten.val[0];
        }
    }
    if (max != -1 && min != 256)
        return max-min;
    return 0;
}

std::vector<Point> Grow(Mat &image_gray, Point &seed, float Thresh, char criterion, int numNeighbors) {
    Vector<int> points;
    Point aux;
    vector<Point> region;
    vector<vector<bool>> verified;
    for (int i = 0; i < image_gray.size().height; i++) {
        verified.push_back(std::vector<bool>(image_gray.size().width, false));
    }
    std::queue<Point> active_front;
    active_front.push(seed);
    image_gray.at<int>(seed.y, seed.x);
    //Growing algorithm
    while (!active_front.empty()) {
        aux = active_front.front();
        if (verified[aux.y][aux.x]) {
            active_front.pop();
            continue;
        }
        region.push_back(aux);
        for (int i = 0; i < 8; i++) {
            if(numNeighbors == 4){
                if(neighbors[i].x == -1 && neighbors[i].y == -1)
                    continue;
                if(neighbors[i].x == 1 && neighbors[i].y == -1)
                    continue;
                if(neighbors[i].x == -1 && neighbors[i].y == 1)
                    continue;
                if(neighbors[i].x == 1 && neighbors[i].y == 1)
                    continue;
            }
            Point actualPoint = aux + neighbors[i];
            //Check image limits
            if (actualPoint.x < 0 || actualPoint.y < 0 || actualPoint.x > image_gray.size().width - 1 ||
                actualPoint.y > image_gray.size().height - 1) {

                continue;
            } else {
                if(criterion == 'a'){
                    if (std::abs((average(actualPoint, image_gray, numNeighbors) - average(aux, image_gray, numNeighbors))) <= Thresh) {
                        active_front.push(actualPoint);
                    }
                }else if(criterion == 'c'){
                    if (std::abs((contrast(actualPoint, image_gray , numNeighbors) - contrast(aux, image_gray, numNeighbors))) <= Thresh) {
                        active_front.push(actualPoint);
                    }
                }else if(criterion == 'v'){
                    if (std::abs((variance(actualPoint, image_gray, numNeighbors) - variance(aux, image_gray, numNeighbors))) <= Thresh) {
                        active_front.push(actualPoint);
                    }
                }
            }
        }
        verified[aux.y][aux.x] = true;
        active_front.pop();
    }
    return region;

}

int main(int argc, char **argv) {
    // Get command line arguments
     bool isAutoS = false;
    if (argc < 6) {
        std::cerr << "Usage: " << argv[0] << " image_file [x y] | autoseed [thresh] [similarity criterion](a|c|v) [number of neighbors](4|8) "<< std::endl;
        return (-1);
    } else if(argc == 6){
        if(strcmp(argv[2],"autoseed")!=0){
            std::cerr << "Usage: " << argv[0] << " image_file [x y] | autoseed [thresh] [similarity criterion](a|c|v) [number of neighbors](4|8) "<< std::endl;
            return (-1);
        } else{
            isAutoS = true;
        }
    } else if(argc == 7){
        if(strcmp(argv[2],"autoseed")==0 || strcmp(argv[3],"autoseed")==0){
            std::cerr << "Usage: " << argv[0] << " image_file [x y] | autoseed [thresh] [similarity criterion](a|c|v) [number of neighbors](4|8) "<< std::endl;
            return (-1);
        }
    }

    // Review given command line arguments
    std::cout << "-----------------------------------------------------" << std::endl;
    std::cout << "Given command line arguments..." << std::endl;
    for (int a = 0; a < argc; a++)
        std::cout << argv[a] << std::endl;
    std::cout << "-----------------------------------------------------" << std::endl;

    // Read an image
    Mat image;
    image = imread(argv[1], 1);

    //Saving Thresh (umbral)
    int umbral = std::atoi(argv[4]);

    if (!image.data) {
        std::cerr << "Error: No image data" << std::endl;
        return (-1);

    } // fi

    //Turn gray scale
    Mat image_gray;
    cvtColor(image, image_gray, COLOR_RGB2GRAY);


    //Setting seed point
    Point seed;
    if (isAutoS){
        seed = Point(-1, -1);
    }else{
        seed = Point(std::atoi(argv[2]), std::atoi(argv[3]));
    }
    if (seed.x == -1 && seed.y == -1) {
        std::cout << "Automatic seed enabled..." << std::endl;
        seed = seedGenerator(image_gray);
    } else {
        if (seed.x > image.size().width - 1 || seed.x > image.size().height - 1) {
            std::cerr << "Seed point is outside image limits ..." << std::endl;
            return (-1);
        }
    }

    char criterion; 
    int nNeighbors=0;
    if(isAutoS==false){
        //Select criterion to compare
        criterion = *argv[5];
        if(criterion != 'a' && criterion != 'c' && criterion != 'v'){
            std::cerr << "Sixth argument must be c for contrast, a for average or v for variance... "<< std::endl;
            return (-1);
        }
        //Saving number of neighbors
        nNeighbors = std::atoi(argv[6]);
        if(nNeighbors != 4 && nNeighbors != 8){
            std::cerr<< "Number of neighbors must be 4 or 8"<<std::endl;
            return (-1);
        }
    }else {
        //Select criterion to compare
        criterion = *argv[4];
        if(criterion != 'a' && criterion != 'c' && criterion != 'v'){
            std::cerr << "Fifth argument must be c for contrast, a for average or v for variance... "<< std::endl;
            return (-1);
        }
        //Saving number of neighbors
        nNeighbors = std::atoi(argv[5]);
        if(nNeighbors != 4 && nNeighbors != 8){
            std::cerr<< "Number of neighbors must be 4 or 8"<<std::endl;
            return (-1);
        }
    }
    

    //Growing
    vector<Point> region = Grow(image_gray, seed, umbral, criterion, nNeighbors);

    //Fill object
    Mat etiquetada = Mat::zeros(image.size(), CV_8UC3);
    Mat segmentada = Mat::zeros(image.size(), CV_8UC3);
    cvtColor(etiquetada, etiquetada, COLOR_RGB2GRAY);
    cvtColor(segmentada, segmentada, COLOR_RGB2GRAY);

    for (int i = 0; i < region.size(); i++) {
        if (region[i].x == 10 && region[i].y == 10) {
            if(isAutoS){
                std::cout << "If the result seems strange, try your own seed ..." << std::endl;
            }
            else{
                std::cout << "If the result seems strange, it is recommended to choose a lower threshold or a different similarity criterion...." << std::endl;
            }
            
        }
        etiquetada.at<uchar>(region[i].y, region[i].x) = 255;
        segmentada.at<uchar>(region[i].y, region[i].x) = image_gray.at<uchar>(region[i].y, region[i].x);
    }

    //Write something
    std::stringstream ss(argv[1]);
    std::string basename;
    getline(ss, basename, '.');
    imwrite(basename + "_etiquetada.png", etiquetada);
    imwrite(basename + "_segmentada.png", segmentada);
    return (0);
}

// eof - RegionGrow.cxx