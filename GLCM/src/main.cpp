/*=================================================================
 * Calculate GLCM(Gray-level Co-occurrence Matrix) By OpenCV.
 *
 * Copyright (C) 2017 Chandler Geng. All rights reserved.
 *
 *     This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 *     You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place, Suite 330, Boston, MA 02111-1307 USA
===================================================================
*/


#include "glcm.h"
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace std;
using namespace cv;


int main()
{
    char key;
    Mat img;
    GLCM glcm;
    TextureEValues EValues;

    // 程序运行时间统计变量
    double time;
    double start;

    // 纹理特征值矩阵
    Mat imgEnergy, imgContrast, imgCorrelation, imgEntropy, imgidMoment;

    // 读取图像
	string path = "C:\\Users\\Ranhui\\Desktop\\ROI\\";
    img = imread(path + "roil.png");

    Mat dstChannel;
    glcm.getOneChannel(img, dstChannel, CHANNEL_B);

    // 灰度量化，并统计运算时间
    start = static_cast<double>(getTickCount());
    glcm.GrayMagnitude(dstChannel, dstChannel, GRAY_8);
    time = ((double)getTickCount() - start) / getTickFrequency() * 1000;
    cout << "Time of Magnitude Gray Image: " << time << "ms" <<endl<<endl;
	
    // 计算整幅图像的纹理特征值图像，并统计运算时间
    start = static_cast<double>(getTickCount());
    glcm.CalcuTextureImages(dstChannel, imgEnergy, imgContrast, imgCorrelation, imgEntropy, imgidMoment, 5, GRAY_8, true);
    time = ((double)getTickCount() - start) / getTickFrequency() * 1000;
    cout << "Time of Generate the whole Image's Calculate Texture Features Image: " << time << "ms" << endl<<endl;

	start = static_cast<double>(getTickCount());
    glcm.CalcuTextureEValue(dstChannel, EValues, 5, GRAY_8);
    time = ((double)getTickCount() - start) / getTickFrequency() * 1000;
    cout << "Time of Calculate Texture Features of the whole Image: " << time << "ms" << endl<<endl;

	//输出纹理特征值
	path = "C:\\Users\\Ranhui\\Desktop\\Evalue\\2\\";
	ofstream dout(path + "result.txt");
    dout <<"Image's Texture EValues:"<<endl;
    dout <<"    Contrast: "<<EValues.contrast<<endl;
    dout <<"    Energy: "<<EValues.energy<<endl;
    dout <<"    Entropy: "<<EValues.entropy<<endl;
    dout <<"    Correlation: "<<EValues.correlation<<endl;
	dout <<"    Idmoment: " << EValues.idMoment << endl;
	cout << "Finish write the feature's txt" << endl;

	//输出纹理图像
	string eg = path + "Energy.jpg";
	string ct = path + "Contrast.jpg";
	string cl = path + "Correlation.jpg";
	string et = path + "Entropy.jpg";
	string im = path + "Idmoment.jpg";
    imwrite(eg, imgEnergy);
    imwrite(ct, imgContrast);
    imwrite(cl, imgCorrelation);
    imwrite(et, imgEntropy);
	imwrite(im, imgidMoment);

    waitKey(60000);

    return 0;
}
