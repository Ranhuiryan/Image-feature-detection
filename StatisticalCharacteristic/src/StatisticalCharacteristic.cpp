//-----------------------------------【程序说明】----------------------------------------------
//		程序名称：图像统计特征提取 
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
static void ShowImageHistgram(Mat g_srcImage);
static void CalStatisticValue(Mat g_srcImage);


int main()
{
	//输入并显示原始图
	g_srcImage = imread("11.tif", 0);
	if (!g_srcImage.data) { printf("Oh，no，载入图像错误！ \n"); return false; }

	namedWindow("【载入图像】");
	imshow("【载入图像】", g_srcImage);

	//计算统计特征
	ShowImageHistgram(g_srcImage);
	CalStatisticValue(g_srcImage);
	waitKey();
	return 0;
}


/*===================================================================
 * 函数名：ShowImageHistgram
 * 说明：生成一维灰度直方图
 * 参数：
 *   Mat g_srcImage:  源图像
 * 返回值：void
 *------------------------------------------------------------------
 */
 static void ShowImageHistgram(Mat g_srcImage)
{
	//载入原图
	Mat srcImage = g_srcImage;
	if (!srcImage.data) { cout << "读取图片错误，请确定目录下是否有指定图片存在！ \n" << endl; }
	MatND dstHist;
	int dims = 1;
	float hranges[] = { 0, 255 };
	const float *ranges[] = { hranges };
	int size = 256;
	int channels = 0;

	//计算图像的直方图
	calcHist(&srcImage, 1, &channels, Mat(), dstHist, dims, &size, ranges);
	int scale = 1;

	Mat dstImage(size * scale, size, CV_8U, Scalar(0));
	double minValue = 0;
	double maxValue = 0;
	minMaxLoc(dstHist, &minValue, &maxValue, 0, 0);

	//绘制出直方图
	int hpt = saturate_cast<int>(0.9 * size);
	for (int i = 0; i < 256; i++)
	{
		float binValue = dstHist.at<float>(i);
		int realValue = saturate_cast<int>(binValue * hpt / maxValue);
		rectangle(dstImage, Point(i*scale, size - 1), Point((i + 1)*scale - 1, size - realValue), Scalar(255));
	}
	imshow("一维灰度直方图", dstImage);
}

/*===================================================================
 * 函数名：CalStatisticValue
 * 说明：计算统计特征值
 * 参数：
 *   Mat g_srcImage:  源图像
 * 返回值：void
 *------------------------------------------------------------------
 */
static void CalStatisticValue(Mat g_srcImage)
{
	Mat src = g_srcImage;

	//计算均值与标准差
	Mat mat_mean, mat_stddev;
	meanStdDev(src, mat_mean, mat_stddev);

	double m, s, n;
	n = src.total();
	m = mat_mean.at<double>(0, 0);
	s = mat_stddev.at<double>(0, 0);

	//计算偏度与峰度
	Mat tmp;
	src.convertTo(tmp, CV_32F);
	float g, k;
	int temp, rows, cols;
	g = k = temp = 0;
		
	for (int i = 0; i < tmp.rows; i++) 
	{
		for (int j = 0; j < tmp.cols; j++) 
		{
			temp = tmp.at<float>(i, j);
			g += (powf(temp - m, 3) / (n*powf(s, 3)));
			k += (powf(temp - m, 4) / (n*powf(s, 4)));
		}
	}
	k -= 3;
	
	//输出结果
	cout << "输入图像的统计特性值：" << endl;
	cout << "	灰度均值：" << m << endl;
	cout << "	标准差：" << s << endl;
	cout << "	偏度：" << g << endl;
	cout << "	峰度：" << k << endl;
	
}
