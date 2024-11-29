#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <unistd.h>

using namespace cv;
using namespace std;


struct MouseParams{
    Mat img;
    int i1;
    int j1;
    int i2;
    int j2;
    int flag = 0;
};

struct hLine{
    float a;
    float b;
    float c;
};

struct MouseParams2{
    Mat img;
    hLine p1;
    hLine l1;
};

hLine getLine(int x1, int y1, int x2, int y2){
    float a = (float)y2 - (float)y1;
    float b = (float)x1 - (float)x2;
    float c = (float)y1*(x2-x1) - (float)x1*(y2-y1);
    struct hLine tLine;
    tLine.a = a/b;
    tLine.b = 1;
    tLine.c = c/b;
    return tLine;
}

hLine getLineIntersec(hLine line1, hLine line2){
    hLine temp;
    temp.a = (line1.b*line2.c) - (line1.c*line2.b);
    temp.b = (line1.c*line2.a) - (line1.a*line2.c);
    temp.c = (line1.a*line2.b) - (line1.b*line2.a);

    if(temp.c!=0){
        temp.a /= temp.c;
        temp.b /= temp.c;
        temp.c = 1;
    }

    return temp;
}


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    MouseParams *vars = (MouseParams *) (userdata);
    Mat timg = vars->img;

    if(event == EVENT_LBUTTONDOWN)
    {
        if(vars->flag==0){

            for(int i = max(0, x-2); i < min(timg.cols, 2+x); i++){
                for(int j = max(0, y-2); j < min(timg.rows, 2+y); j++){
                    timg.at<uchar>(j, i) = 255;
                }
            }
            vars->i1 = x;
            vars->j1 = y;
            vars->flag = 1;
        }
        else if(vars->flag==1){

            float iT = (float)vars->i1;
            float jT = (float)vars->j1;

            line(timg, Point( iT, jT ), Point( x, y), Scalar( 255, 255, 255 ),  2, 8 );

            /* Line Drawing function
            int count = 0;

            while(iT!=x && jT != y && count < 1000){
                iT = iT + ((float)x - vars->i1)/1000.0;
                jT = jT + ((float)y - vars->j1)/1000.0;
                count++;
                
                for(int i = max(0, (int)iT-2); i < min(timg.cols, 2+(int)iT); i++){
                    for(int j = max(0, (int)jT-2); j < min(timg.rows, 2+(int)jT); j++){
                        timg.at<uchar>(j, i) = 255;
                    }
                }
            }
            */
            
            for(int i = max(0, x-2); i < min(timg.cols, 2+x); i++){
                for(int j = max(0, y-2); j < min(timg.rows, 2+y); j++){
                    timg.at<uchar>(j, i) = 255;
                }
            }

            vars->i2 = x;
            vars->j2 = y;
            vars->flag = 2;
        }
    }

    imshow("My Window", timg);
}

hLine getCenterLine(Mat image, hLine line1){

    int p1 = image.cols / 2;
    int p2 = image.rows / 2;

    hLine passCenter;
    passCenter.a = line1.a;
    passCenter.b = line1.b;
    passCenter.c = -((line1.a * p1) + (line1.b * p2));

    return passCenter;

}


void drawLine(Mat image, hLine line1){
    hLine x0;
    x0.a = 1; x0.b = 0; x0.c = 0;
    hLine y0;
    y0.a = 0; y0.b = 1; y0.c = 0;
    hLine xl;
    xl.a = 1; xl.b = 0; xl.c = -(image.cols - 1);
    hLine yl;
    yl.a = 0; yl.b = 1; yl.c = -(image.rows - 1);

    hLine pts[4];
    pts[0] = getLineIntersec(line1, x0);
    pts[1] = getLineIntersec(line1, y0);
    pts[2] = getLineIntersec(line1, xl);
    pts[3] = getLineIntersec(line1, yl);

    int flag = -1;

    float iT;
    float jT;
    float x;
    float y;

    for(int i = 0; i < 4; i++){
        if(pts[i].a >= 0 && pts[i].a < image.cols && pts[i].b >= 0 && pts[i].b < image.rows && flag != i){
            if(flag < 0){
                iT = pts[i].a;
                jT = pts[i].b;
                flag = i;
            }
            else{
                x = pts[i].a;
                y = pts[i].b;
                flag = 5;
            }
        }
    }

    if(flag != 5){
        cout << "Cannot plot line." << endl;
        return;
    }

    line(image, Point( iT, jT ), Point( x, y), Scalar( 255, 255, 255 ),  2, 8 );

}


void CallBackFunc2(int event, int x, int y, int flags, void* userdata)
{
    MouseParams2 *vars = (MouseParams2 *) (userdata);
    Mat timg = vars->img.clone();   

    if(event == EVENT_MOUSEMOVE)
    {
        {
            hLine vLine;
            vLine.a = 1;
            vLine.b = 0;
            vLine.c = -x; 

            hLine cPoint = getLineIntersec(vLine, vars->l1);

            for(int i = max(0, (int)cPoint.a - 5); i < min(timg.cols, 5 + (int)cPoint.a); i++){
                for(int j = max(0, (int)cPoint.b - 5); j < min(timg.rows, 5 + (int)cPoint.b); j++){
                    timg.at<uchar>(j, i) = 255;
                }
            }

            hLine printLine = getLineIntersec(cPoint, vars->p1);

            drawLine(timg, printLine);
        }

        {
            hLine vLine;
            vLine.a = 1;
            vLine.b = 0;
            vLine.c = -x; 

            hLine cPoint = getLineIntersec(vLine, vars->l1);
            cPoint.a = min(timg.cols, (int)cPoint.a + 20);

            for(int i = max(0, (int)cPoint.a - 5); i < min(timg.cols, 5 + (int)cPoint.a); i++){
                for(int j = max(0, (int)cPoint.b - 5); j < min(timg.rows, 5 + (int)cPoint.b); j++){
                    timg.at<uchar>(j, i) = 255;
                }
            }

            hLine printLine = getLineIntersec(cPoint, vars->p1);

            drawLine(timg, printLine);
        }

        {
            hLine vLine;
            vLine.a = 1;
            vLine.b = 0;
            vLine.c = -x; 

            hLine cPoint = getLineIntersec(vLine, vars->l1);
            cPoint.a = max(0, (int)cPoint.a - 20);

            for(int i = max(0, (int)cPoint.a - 5); i < min(timg.cols, 5 + (int)cPoint.a); i++){
                for(int j = max(0, (int)cPoint.b - 5); j < min(timg.rows, 5 + (int)cPoint.b); j++){
                    timg.at<uchar>(j, i) = 255;
                }
            }

            hLine printLine = getLineIntersec(cPoint, vars->p1);

            drawLine(timg, printLine);
        }

    }

    imshow("My Window", timg);
}



int main(int argc, char** argv){

    String imageName("Garden.JPG");
    if( argc > 1)
    {
        imageName = argv[1];
    }

    // Read image from file     
    Mat imageMain = imread(imageName);
    //if fail to read the image
    if ( imageMain.empty() ) 
    { 
        cout << "Error loading the image" << endl;
        return -1; 
    }

    while(1){

        system("clear");

        int input = 0;
        cout << "Choose action: (enter number) " << endl;
        cout << "\t1. Get Vanishing Point from two lines (standalone)(part 1)" << endl;
        cout << "\t2. Get Vanishing Line using two lines (standalone)(part 2, 3 & 4)" << endl;
        cout << "\t3. Affine Rectification using two lines (standalone)(part 5)" << endl;

        cin >> input;

        if(input == 1){

            MouseParams mp;
            mp.img = imageMain.clone();
            cvtColor( mp.img, mp.img, COLOR_BGR2GRAY );

            cout << "Click two points to mark line 1 (press enter to continue) " << endl;
            cin.get();

            namedWindow("My Window", 1);
            imshow("My Window", mp.img);
            setMouseCallback("My Window", CallBackFunc, (void*)&mp);
            imshow("My Window", mp.img);
            waitKey(0);

            hLine line1 = getLine(mp.i1, mp.j1, mp.i2, mp.j2);
            cout << "Given Line: y = " << -line1.a << "x + " << -line1.c << endl;

            cout << "Click two points to mark line 2 (press enter to continue) " << endl;
            cin.get();

            mp.flag = 0;
            namedWindow("My Window", 1);
            imshow("My Window", mp.img);
            setMouseCallback("My Window", CallBackFunc, (void*)&mp);
            imshow("My Window", mp.img);
            waitKey(0);

            hLine line2 = getLine(mp.i1, mp.j1, mp.i2, mp.j2);
            cout << "Given Line: y = " << -line2.a << "x + " << -line2.c << endl;

            hLine point1 = getLineIntersec(line1, line2);

            cout << "Show vanishing point (with shape of Square Blob. Press enter to continue): " << endl;
            cin.get();      

            int tempX = point1.a;
            int tempY = point1.b;      

            for(int i = max(0, (int)tempX-5); i < min(mp.img.cols, 5+(int)tempX); i++){
                for(int j = max(0, (int)tempY-5); j < min(mp.img.rows, 5+(int)tempY); j++){
                    mp.img.at<uchar>(j, i) = 255;
                }
            }

            imshow("My Window", mp.img);
            waitKey(0);

        }


        else if(input == 2){

            MouseParams mp;
            mp.img = imageMain.clone();
            cvtColor( mp.img, mp.img, COLOR_BGR2GRAY );

            cout << "Input 2 sets of parallel lines." << endl;

            cout << "Click two points to mark line 1" << endl;

            namedWindow("My Window", 1);
            imshow("My Window", mp.img);
            setMouseCallback("My Window", CallBackFunc, (void*)&mp);
            imshow("My Window", mp.img);
            waitKey(0);

            hLine line1 = getLine(mp.i1, mp.j1, mp.i2, mp.j2);
            cout << "Given Line: y = " << -line1.a << "x + " << -line1.c << endl;

            cout << "Click two points to mark line 2" << endl;

            mp.flag = 0;
            namedWindow("My Window", 1);
            imshow("My Window", mp.img);
            setMouseCallback("My Window", CallBackFunc, (void*)&mp);
            imshow("My Window", mp.img);
            waitKey(0);

            hLine line2 = getLine(mp.i1, mp.j1, mp.i2, mp.j2);
            cout << "Given Line: y = " << -line2.a << "x + " << -line2.c << endl;

            cout << "Click two points to mark line 3" << endl;

            mp.flag = 0;
            namedWindow("My Window", 1);
            imshow("My Window", mp.img);
            setMouseCallback("My Window", CallBackFunc, (void*)&mp);
            imshow("My Window", mp.img);
            waitKey(0);

            hLine line3 = getLine(mp.i1, mp.j1, mp.i2, mp.j2);
            cout << "Given Line: y = " << -line3.a << "x + " << -line3.c << endl;

            cout << "Click two points to mark line 4" << endl;

            mp.flag = 0;
            namedWindow("My Window", 1);
            imshow("My Window", mp.img);
            setMouseCallback("My Window", CallBackFunc, (void*)&mp);
            imshow("My Window", mp.img);
            waitKey(0);

            hLine line4 = getLine(mp.i1, mp.j1, mp.i2, mp.j2);
            cout << "Given Line: y = " << -line4.a << "x + " << -line4.c << endl;

            hLine point1 = getLineIntersec(line1, line2);
            hLine point2 = getLineIntersec(line3, line4);

            cout << "Show vanishing line (Press enter to continue): " << endl;
            cin.get(); 

            hLine vanishLine = getLineIntersec(point1, point2);
            drawLine(mp.img, vanishLine);
            imshow("My Window", mp.img);
            waitKey(0);

            hLine vanishLineC = getCenterLine(mp.img, vanishLine);

            cout << "Show vanishing line passing thorugh center of image (Press enter to continue): " << endl;
            cin.get(); 

            cvtColor( imageMain, mp.img, COLOR_BGR2GRAY );
            drawLine(mp.img, vanishLineC);
            imshow("My Window", mp.img);
            waitKey(0);

            MouseParams2 mp2;
            mp2.img = mp.img.clone();
            if(point1.a >= 0 && point1.a <= mp.img.cols){
                mp2.p1 =  point1;
            }
            else{
                mp2.p1 = point2;
            }

            mp2.l1 = vanishLineC;

            cout << "Show parallel lines connecting points in L and V (Press enter to continue): " << endl;
            cin.get(); 

            namedWindow("My Window", 1);
            imshow("My Window", mp2.img);
            setMouseCallback("My Window", CallBackFunc2, (void*)&mp2);
            imshow("My Window", mp2.img);
            waitKey(0);




        }


        else if(input==3){
            MouseParams mp;
            Mat imageMain2 = imread("painting.jpg");
            if ( imageMain2.empty() ) 
            { 
                cout << "Error loading the painting image." << endl;
                return -1; 
            }
            mp.img = imageMain2.clone();
            cvtColor( mp.img, mp.img, COLOR_BGR2GRAY );

            cout << "Input 2 sets of parallel lines." << endl;

            cout << "Click two points to mark line 1" << endl;

            namedWindow("My Window", 1);
            imshow("My Window", mp.img);
            setMouseCallback("My Window", CallBackFunc, (void*)&mp);
            imshow("My Window", mp.img);
            waitKey(0);

            hLine line1 = getLine(mp.i1, mp.j1, mp.i2, mp.j2);
            cout << "Given Line: y = " << -line1.a << "x + " << -line1.c << endl;

            cout << "Click two points to mark line 2" << endl;

            mp.flag = 0;
            namedWindow("My Window", 1);
            imshow("My Window", mp.img);
            setMouseCallback("My Window", CallBackFunc, (void*)&mp);
            imshow("My Window", mp.img);
            waitKey(0);

            hLine line2 = getLine(mp.i1, mp.j1, mp.i2, mp.j2);
            cout << "Given Line: y = " << -line2.a << "x + " << -line2.c << endl;

            cout << "Click two points to mark line 3" << endl;

            mp.flag = 0;
            namedWindow("My Window", 1);
            imshow("My Window", mp.img);
            setMouseCallback("My Window", CallBackFunc, (void*)&mp);
            imshow("My Window", mp.img);
            waitKey(0);

            hLine line3 = getLine(mp.i1, mp.j1, mp.i2, mp.j2);
            cout << "Given Line: y = " << -line3.a << "x + " << -line3.c << endl;

            cout << "Click two points to mark line 4" << endl;

            mp.flag = 0;
            namedWindow("My Window", 1);
            imshow("My Window", mp.img);
            setMouseCallback("My Window", CallBackFunc, (void*)&mp);
            imshow("My Window", mp.img);
            waitKey(0);

            hLine line4 = getLine(mp.i1, mp.j1, mp.i2, mp.j2);
            cout << "Given Line: y = " << -line4.a << "x + " << -line4.c << endl;

            hLine point1 = getLineIntersec(line1, line2);
            hLine point2 = getLineIntersec(line3, line4);

            hLine vanishLine = getLineIntersec(point1, point2);
            
            //Homography for affine rectification based on vanishing line
            float hArr[3][3];
            hArr[0][0] = 0.5;
            hArr[0][1] = 0;
            hArr[0][2] = 0;
            hArr[1][0] = 0;
            hArr[1][1] = 0.5;
            hArr[1][2] = 0;
            hArr[2][0] = vanishLine.a;
            hArr[2][1] = vanishLine.b;
            hArr[2][2] = vanishLine.c;

            cout << vanishLine.a << " " << vanishLine.b << " " << vanishLine.c;

            Mat H = Mat(3, 3, CV_32F, hArr);
            warpPerspective(mp.img, mp.img, H, mp.img.size());    

            cout << "Show vanishing line (Press enter to continue): " << endl;
            cin.get();         

            imshow("My Window", mp.img);
            waitKey(0);
        }




    }


    return 0;
}
