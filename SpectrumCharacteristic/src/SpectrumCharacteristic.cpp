//-----------------------------------【程序说明】----------------------------------------------
//		程序名称：图像频谱特征提取 
//		开发所用IDE版本：Visual Studio 2017
//   	开发所用OpenCV版本：4.0.1
//		编译日期：2019年3月27日 
//----------------------------------------------------------------------------------------------


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace std;
using namespace cv;


Mat g_srcImage;//原始图
static void SpectrumCharacteristic(Mat g_srcImage);


int main()
{
	g_srcImage = imread("11.tif", 0);
	if (!g_srcImage.data) { printf("Oh，no，载入图像错误！ \n"); return false; }

	namedWindow("【载入图像】");
	imshow("【载入图像】", g_srcImage);

	namedWindow("【频谱特征】", 1);
	SpectrumCharacteristic(g_srcImage);
	waitKey(6000);
	return 0;
}


/*===================================================================
 * 函数名：SpectrumCharacteristic
 * 说明：生成图像的二维傅里叶变换图
 * 参数：
 *   Mat g_srcImage:  源图像
 * 返回值：void
 *------------------------------------------------------------------
 */
static void SpectrumCharacteristic(Mat g_srcImage)
{
	Mat srcImage = g_srcImage;
	if (!srcImage.data) { printf("读取图片错误，请确定目录下是否有imread函数指定图片存在！ \n"); }

	int m = getOptimalDFTSize(srcImage.rows);
	int n = getOptimalDFTSize(srcImage.cols);
	Mat padded;
	copyMakeBorder(srcImage, padded, 0, m - srcImage.rows, 0, n - srcImage.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI);

	dft(complexI, complexI);

	split(complexI, planes);
	magnitude(planes[0], planes[1], planes[0]);
	Mat magnitudeImage = planes[0];

	magnitudeImage += Scalar::all(1);
	log(magnitudeImage, magnitudeImage);

	magnitudeImage = magnitudeImage(Rect(0, 0, magnitudeImage.cols & -2, magnitudeImage.rows & -2));
	int cx = magnitudeImage.cols / 2;
	int cy = magnitudeImage.rows / 2;
	Mat q0(magnitudeImage, Rect(0, 0, cx, cy));   // ROI区域的左上
	Mat q1(magnitudeImage, Rect(cx, 0, cx, cy));  // ROI区域的右上
	Mat q2(magnitudeImage, Rect(0, cy, cx, cy));  // ROI区域的左下
	Mat q3(magnitudeImage, Rect(cx, cy, cx, cy)); // ROI区域的右下
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(magnitudeImage, magnitudeImage, 0, 1, NORM_MINMAX);

	imshow("【频谱特征】", magnitudeImage);

}
