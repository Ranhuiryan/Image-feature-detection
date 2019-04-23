//-----------------------------------������˵����----------------------------------------------
//		�������ƣ�ͼ��ͳ��������ȡ 
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
static void ShowImageHistgram(Mat g_srcImage);
static void CalStatisticValue(Mat g_srcImage);


int main()
{
	//���벢��ʾԭʼͼ
	g_srcImage = imread("11.tif", 0);
	if (!g_srcImage.data) { printf("Oh��no������ͼ����� \n"); return false; }

	namedWindow("������ͼ��");
	imshow("������ͼ��", g_srcImage);

	//����ͳ������
	ShowImageHistgram(g_srcImage);
	CalStatisticValue(g_srcImage);
	waitKey();
	return 0;
}


/*===================================================================
 * ��������ShowImageHistgram
 * ˵��������һά�Ҷ�ֱ��ͼ
 * ������
 *   Mat g_srcImage:  Դͼ��
 * ����ֵ��void
 *------------------------------------------------------------------
 */
 static void ShowImageHistgram(Mat g_srcImage)
{
	//����ԭͼ
	Mat srcImage = g_srcImage;
	if (!srcImage.data) { cout << "��ȡͼƬ������ȷ��Ŀ¼���Ƿ���ָ��ͼƬ���ڣ� \n" << endl; }
	MatND dstHist;
	int dims = 1;
	float hranges[] = { 0, 255 };
	const float *ranges[] = { hranges };
	int size = 256;
	int channels = 0;

	//����ͼ���ֱ��ͼ
	calcHist(&srcImage, 1, &channels, Mat(), dstHist, dims, &size, ranges);
	int scale = 1;

	Mat dstImage(size * scale, size, CV_8U, Scalar(0));
	double minValue = 0;
	double maxValue = 0;
	minMaxLoc(dstHist, &minValue, &maxValue, 0, 0);

	//���Ƴ�ֱ��ͼ
	int hpt = saturate_cast<int>(0.9 * size);
	for (int i = 0; i < 256; i++)
	{
		float binValue = dstHist.at<float>(i);
		int realValue = saturate_cast<int>(binValue * hpt / maxValue);
		rectangle(dstImage, Point(i*scale, size - 1), Point((i + 1)*scale - 1, size - realValue), Scalar(255));
	}
	imshow("һά�Ҷ�ֱ��ͼ", dstImage);
}

/*===================================================================
 * ��������CalStatisticValue
 * ˵��������ͳ������ֵ
 * ������
 *   Mat g_srcImage:  Դͼ��
 * ����ֵ��void
 *------------------------------------------------------------------
 */
static void CalStatisticValue(Mat g_srcImage)
{
	Mat src = g_srcImage;

	//�����ֵ���׼��
	Mat mat_mean, mat_stddev;
	meanStdDev(src, mat_mean, mat_stddev);

	double m, s, n;
	n = src.total();
	m = mat_mean.at<double>(0, 0);
	s = mat_stddev.at<double>(0, 0);

	//����ƫ������
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
	
	//������
	cout << "����ͼ���ͳ������ֵ��" << endl;
	cout << "	�ҶȾ�ֵ��" << m << endl;
	cout << "	��׼�" << s << endl;
	cout << "	ƫ�ȣ�" << g << endl;
	cout << "	��ȣ�" << k << endl;
	
}
