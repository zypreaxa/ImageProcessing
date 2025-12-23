#ifndef QT_IMAGE_VIEWER_MAINWINDOW_HPP
#define QT_IMAGE_VIEWER_MAINWINDOW_HPP

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QScrollArea>

#include <QSplitter>
#include <QChartView>

#include <image.hpp>

class QtImageViewer : public QMainWindow{

	Q_OBJECT

public:
	QtImageViewer(QWidget *parent=nullptr);
	QtImageViewer(const QString filename, QWidget *parent=nullptr);
	
	virtual ~QtImageViewer();
			  
public slots:
	//void showFile(const QString filename);
	void showImageLeft(Image<uint8_t>*img);
	void showImageRight(Image<uint8_t> *img);
	void openFile();
	void clearFile();
	void quit();
	void combineImagesRGB();
	void togreyscale();
	void negate();
	void negateLUT();
	void powerlaw();
	void powerlawLUT();
	void linear();
	void linearLUT();
	void thresholding();
	void thresholdingLUT();
	void histogrameq();
	void lowpass();
	void median();
	void laplacian();
private:
	enum ImageView{
		NONE = 0,
		LEFT = 2<<0,
		RIGHT = 2<<1,
	};

	void init();
	void createActions();
	void createMenus();
	
	QSplitter *_mainSplitter{nullptr};

	unsigned int _imageState{ImageView::NONE};

	void showImage(Image<uint8_t>* img, ImageView view);

	void setCurrentImage(Image<uint8_t>* img);
	
	QImage _lImage;
	QLabel *_lImageLabel{nullptr};
	QScrollArea *_lScrollArea{nullptr};
	QSplitter *_leftSplitter{nullptr};
	QtCharts::QChartView *_lChartView{nullptr};
	
	QImage _rImage;
	QLabel *_rImageLabel{nullptr};
	QScrollArea *_rScrollArea{nullptr};
	QSplitter *_rightSplitter{nullptr};
	QtCharts::QChartView *_rChartView{nullptr};
	
	QMenu *_fileMenu{nullptr};
	QAction *_fileOpenAction{nullptr};
	QAction *_fileClearAction{nullptr};
	QAction *_quitAction{nullptr};
	

	QMenu *_trMenu{nullptr};
	QAction *_trFourierTransform{nullptr};
	QAction *_trTogreyscale{nullptr};
	QAction *_trCombine{nullptr};
	QAction *_trNegate{nullptr};
	QAction *_trNegateLUT{nullptr};
	QAction *_trPowerlaw{nullptr};
	QAction *_trPowerlawLUT{nullptr};
	QAction *_trLinear{nullptr};
	QAction *_trLinearLUT{nullptr};
	QAction *_trThresholding{nullptr};
	QAction *_trThresholdingLUT{nullptr};
	QAction *_trHistogrameq{nullptr};

	QMenu *_spatialMenu{nullptr};
	QAction *_spatialLowpass{nullptr};
	QAction *_spatialMedian{nullptr};
	QAction *_spatialLaplacian{nullptr};

	QMenu *_toolsMenu{nullptr};

	Image<uint8_t> *_currentImage{nullptr};
};

#endif
