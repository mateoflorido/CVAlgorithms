#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <queue>
#include <map>
#include <random>

using namespace cv;
const Point neighbors[8] = {Point(1, 0), Point(1, -1), Point(-1, -1), Point(-1, 0), Point(-1, 1), Point(0, 1),
                            Point(0, -1), Point(1, 1)};

int main(int argc, char **argv) {
    // Get command line arguments
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " image_file" << std::endl;
        return (-1);

    } // fi

    // Review given command line arguments
    std::cout << "-------------------------" << std::endl;
    for (int a = 0; a < argc; a++)
        std::cout << argv[a] << std::endl;
    std::cout << "-------------------------" << std::endl;

    // Read an image
    Mat image;
    image = imread(argv[1], 1);
    if (!image.data) {
        std::cerr << "Error: No image data" << std::endl;
        return (-1);

    } // fi
    //Save basename
    std::stringstream ss(argv[1]);
    std::string basename;
    getline(ss, basename, '.');
    cvtColor(image, image, COLOR_RGB2GRAY);
    Mat labeled = Mat::zeros(image.size(), CV_8UC3);
    std::queue<Point> cola;
    Point aux;
    std::vector<int> regions;
    int contLabels = 1;
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            if (image.at<uchar>(y, x) == 255 && labeled.at<uchar>(y, x) == 0) {
                image.at<uchar>(y, x) = contLabels;
                aux = Point(x, y);
                cola.push(aux);
                regions.push_back(1);
                labeled.at<uchar>(y, x) = 1;
                while (!cola.empty()) {
                    aux = cola.front(); // Espera que no estoy escuchando !!!!!
                    cola.pop();
                    for (int i = 0; i < 8; i++) {
                        Point actualPoint = aux + neighbors[i];
                        if (actualPoint.x < 0 || actualPoint.y < 0 || actualPoint.x > image.size().width - 1 ||
                            actualPoint.y > image.size().height - 1) {
                            continue;
                        } else {
                            if (image.at<uchar>(actualPoint) == 255 && labeled.at<uchar>(actualPoint) == 0) {
                                image.at<uchar>(actualPoint) = contLabels;
                                cola.push(actualPoint);
                                regions[regions.size() - 1] += 1;
                                labeled.at<uchar>(actualPoint) = 1;
                            }
                        }
                    }
                }
                contLabels += 1;
            }
        }
    }

    int max = (*std::max_element(regions.begin(), regions.end()));
    std::cout << "Bigger region found: " << max << std::endl;
    Mat color_image = Mat::zeros(image.size(), CV_8UC3);
    Mat bin_image = Mat::zeros(image.size(), CV_8UC3);
    cvtColor(bin_image, bin_image, COLOR_RGB2GRAY);
    std::map<int, Vec3b> regionColors;
    Vec3b color;
    max *= 0.15;
    std::map<int, int> toDelete;
    auto it = regionColors.begin();
    std::map<int, int> hRegions;
    std::map<int, Point> pRegions;

    for (int i = 0; i < regions.size(); i++) {
        if (regions[i] <= max) {
            toDelete.insert(std::pair<int, int>((i + 1), regions[i]));
            std::cout << "The region number " << i + 1 << " is dropped with size ->" << regions[i] << std::endl;
        } else {
            std::random_device rd{};
            std::mt19937 generator(rd());
            std::uniform_int_distribution<uchar> distribution(1, 254);
            color.val[0] = distribution(generator);
            color.val[1] = distribution(generator);
            color.val[2] = distribution(generator);
            regionColors.insert(std::pair<int, Vec3b>(i + 1, color));
            std::cout << "The region number " << i + 1 << " has been accepted with size ->" << regions[i] << std::endl;
            hRegions.insert(std::pair<int, int>(i + 1, 0));
            pRegions.insert(std::pair<int, Point>(i + 1, Point(0, 0)));
        }
    }
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            if (toDelete.find(image.at<uchar>(y, x)) != toDelete.end()) {
                image.at<uchar>(y, x) = 0;
            } else {
                it = regionColors.find(image.at<uchar>(y, x));
                if (it != regionColors.end()) {
                    bin_image.at<uchar>(y, x) = 255;
                    color = color_image.at<Vec3b>(Point(x, y));
                    color.val[0] = it->second.val[0];
                    color.val[1] = it->second.val[1];
                    color.val[2] = it->second.val[2];
                    color_image.at<Vec3b>(Point(x, y)) = color;
                }
            }
        }
    }
    Mat dist;
    distanceTransform(bin_image, dist, CV_DIST_C, 3);
    int invDistance;
    auto itH = hRegions.begin();
    auto itP = pRegions.begin();
    int xi, yi;

    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            it = regionColors.find(image.at<uchar>(y, x));
            if (it != regionColors.end()) {
                invDistance = (int) dist.at<uchar>(y, x);
                itH = hRegions.find(image.at<uchar>(y, x));
                if (itH->second < invDistance) {
                    itH->second = invDistance;
                    itP = pRegions.find(image.at<uchar>(y, x));
                    itP->second = Point(x, y);
                } else if (itH->second == invDistance) {
                    xi = (itP->second.x + x) / 2;
                    yi = (itP->second.y + y) / 2;
                    itP->second = Point(xi, yi);
                }
            }
        }
    }

    itP = pRegions.begin();
    for (; itP != pRegions.end(); itP++) {
        std::cout << "Region #" << itP->first << " has center at " << itP->second << std::endl;
    }
    imwrite(basename + "_CharacterizedInt.png", image);
    imwrite(basename + "_CharacterizedColors.png", color_image);
    imwrite(basename + "_CharacterizedDistanceMap.png", dist);
    return (0);
}

// eof - characterization.cxx
