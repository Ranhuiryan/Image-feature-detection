//-----------------------------------������˵����----------------------------------------------
//		�������ƣ�ͼ��Ƶ��������ȡ 
//		��������IDE�汾��Visual Studio 2017
//   	��������OpenCV�汾��4.0.1
//		�������ڣ�2019��3��27�� 
//----------------------------------------------------------------------------------------------


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace std;
using namespace cv;


Mat g_srcImage;//ԭʼͼ
static void SpectrumCharacteristic(Mat g_srcImage);


int main()
{
	g_srcImage = imread("11.tif", 0);
	if (!g_srcImage.data) { printf("Oh��no������ͼ����� \n"); return false; }

	namedWindow("������ͼ��");
	imshow("������ͼ��", g_srcImage);

	namedWindow("��Ƶ��������", 1);
	SpectrumCharacteristic(g_srcImage);
	waitKey(6000);
	return 0;
}


/*===================================================================
 * ��������SpectrumCharacteristic
 * ˵��������ͼ��Ķ�ά����Ҷ�任ͼ
 * ������
 *   Mat g_srcImage:  Դͼ��
 * ����ֵ��void
 *------------------------------------------------------------------
 */
static void SpectrumCharacteristic(Mat g_srcImage)
{
	Mat srcImage = g_srcImage;
	if (!srcImage.data) { printf("��ȡͼƬ������ȷ��Ŀ¼���Ƿ���imread����ָ��ͼƬ���ڣ� \n"); }

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
	Mat q0(magnitudeImage, Rect(0, 0, cx, cy));   // ROI���������
	Mat q1(magnitudeImage, Rect(cx, 0, cx, cy));  // ROI���������
	Mat q2(magnitudeImage, Rect(0, cy, cx, cy));  // ROI���������
	Mat q3(magnitudeImage, Rect(cx, cy, cx, cy)); // ROI���������
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(magnitudeImage, magnitudeImage, 0, 1, NORM_MINMAX);

	imshow("��Ƶ��������", magnitudeImage);

}