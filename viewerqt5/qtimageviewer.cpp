#include "qtimageviewer.hpp"

#include <QFileDialog>
#include <QMainWindow>
#include <QPalette>
#include <QMenu>
#include <QMenuBar>
#include <QImageReader>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>

#include <tiffloader.hpp> /// include your image loader
#include <imageprocessor.hpp> // including image processor

#include <iostream>

QtImageViewer::QtImageViewer(QWidget *parent) :
  QMainWindow(parent){
  init();
};

void QtImageViewer::init(){
	_lImageLabel = new QLabel;
	_lImageLabel->setBackgroundRole(QPalette::Base);
	_lImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	_lImageLabel->setScaledContents(false);

	_lScrollArea = new QScrollArea;
	_lScrollArea->setBackgroundRole(QPalette::Dark);
	_lScrollArea->setWidget(_lImageLabel);
	_lScrollArea->setVisible(true);

	_rImageLabel = new QLabel;
	_rImageLabel->setBackgroundRole(QPalette::Base);
	_rImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	_rImageLabel->setScaledContents(false);

	_rScrollArea = new QScrollArea;
	_rScrollArea->setBackgroundRole(QPalette::Dark);
	_rScrollArea->setWidget(_rImageLabel);
	_rScrollArea->setVisible(true);

	_mainSplitter = new QSplitter();
	_leftSplitter = new QSplitter(Qt::Vertical);
	_rightSplitter = new QSplitter(Qt::Vertical);

	_mainSplitter->addWidget(_leftSplitter);
	_mainSplitter->addWidget(_rightSplitter);

	_leftSplitter->addWidget(_lScrollArea);
	_rightSplitter->addWidget(_rScrollArea);

	setCentralWidget(_mainSplitter);
	createActions();
};

QtImageViewer::~QtImageViewer(){
	delete(_lImageLabel);
	delete(_lScrollArea);
	delete(_rImageLabel);
	delete(_rScrollArea);
	delete(_fileMenu);
	delete(_fileOpenAction);
	delete(_fileClearAction);
	delete(_quitAction);

	delete(_currentImage);
};

void QtImageViewer::createActions(){

	_fileOpenAction = new QAction(tr("&Open..."), this);
	_fileOpenAction->setShortcut(QKeySequence::Open);
	connect(_fileOpenAction, SIGNAL(triggered()), this, SLOT(openFile()));

	_fileClearAction = new QAction(tr("&Clear display..."), this);
	_fileClearAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Delete));
	connect(_fileClearAction, SIGNAL(triggered()), this, SLOT(clearFile()));
	
	_quitAction = new QAction(tr("&Quit..."), this);
	_quitAction->setShortcut(QKeySequence::Quit);
	connect(_quitAction, SIGNAL(triggered()), this, SLOT(quit()));

	_trFourierTransform = new QAction(tr("&Fourier transform..."), this);
	_trFourierTransform->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));

	_trTogreyscale = new QAction(tr("Convert image to greyscale..."), this);
	_trTogreyscale->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
	connect(_trTogreyscale, SIGNAL(triggered()), this, SLOT(togreyscale()));

	_trCombine = new QAction(tr("&Combine images into RGB..."), this);
	_trCombine->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));
	connect(_trCombine, SIGNAL(triggered()), this, SLOT(combineImagesRGB()));
	
	_trNegate = new QAction(tr("Image negation..."), this);
	_trNegate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
	connect(_trNegate, SIGNAL(triggered()), this, SLOT(negate()));

	_trNegateLUT = new QAction(tr("Image negation using LUT..."), this);
	_trNegateLUT->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_N));
	connect(_trNegateLUT, SIGNAL(triggered()), this, SLOT(negateLUT()));

	_trPowerlaw = new QAction(tr("Power law..."), this);
	_trPowerlaw->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
	connect(_trPowerlaw, SIGNAL(triggered()), this, SLOT(powerlaw()));

	_trPowerlawLUT = new QAction(tr("Power law using LUT..."), this);
	_trPowerlawLUT->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_P));
	connect(_trPowerlawLUT, SIGNAL(triggered()), this, SLOT(powerlawLUT()));

	_trLinear = new QAction(tr("Piece-wise linear..."), this);
	_trLinear->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
	connect(_trLinear, SIGNAL(triggered()), this, SLOT(linear()));

	_trLinearLUT = new QAction(tr("Linear using LUT..."), this);
	_trLinearLUT->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_L));
	connect(_trLinearLUT, SIGNAL(triggered()), this, SLOT(linearLUT()));

	_trThresholding = new QAction(tr("Thresholding..."), this);
	_trThresholding->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_T));
	connect(_trThresholding, SIGNAL(triggered()), this, SLOT(thresholding()));

	_trThresholdingLUT = new QAction(tr("Thresholding using LUT..."), this);
	_trThresholdingLUT->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_T));
	connect(_trThresholdingLUT, SIGNAL(triggered()), this, SLOT(thresholdingLUT()));

	_trHistogrameq = new QAction(tr("Histogram equalization..."), this);
	_trHistogrameq->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_H));
	connect(_trHistogrameq, SIGNAL(triggered()), this, SLOT(histogrameq()));

	_spatialLowpass = new QAction(tr("Lowpass filtering..."), this);
	_spatialLowpass->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
	connect(_spatialLowpass, SIGNAL(triggered()), this, SLOT(lowpass()));

	_spatialMedian = new QAction(tr("Median filtering..."), this);
	_spatialMedian->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
	connect(_spatialMedian, SIGNAL(triggered()), this, SLOT(median()));

	_spatialLaplacian = new QAction(tr("Laplacian transform..."), this);
	_spatialLaplacian->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_X));
	connect(_spatialLaplacian, SIGNAL(triggered()), this, SLOT(laplacian()));

	_fileMenu = menuBar()->addMenu(tr("&File"));
	_fileMenu->addAction(_fileOpenAction);
	_fileMenu->addAction(_fileClearAction);
	_fileMenu->addAction(_quitAction);

	_trMenu = menuBar()->addMenu(tr("&Transforms"));
	_trMenu->addAction(_trFourierTransform);
	_trMenu->addAction(_trTogreyscale);
	_trMenu->addAction(_trCombine);
	_trMenu->addAction(_trNegate);
	_trMenu->addAction(_trNegateLUT);
	_trMenu->addAction(_trPowerlaw);
	_trMenu->addAction(_trPowerlawLUT);
	_trMenu->addAction(_trLinear);
	_trMenu->addAction(_trLinearLUT);
	_trMenu->addAction(_trThresholding);
	_trMenu->addAction(_trThresholdingLUT);
	_trMenu->addAction(_trHistogrameq);
	
	_spatialMenu = menuBar()->addMenu(tr("&Spatial filtering"));
	_spatialMenu->addAction(_spatialLowpass);
	_spatialMenu->addAction(_spatialMedian);
	_spatialMenu->addAction(_spatialLaplacian);

	_toolsMenu = menuBar()->addMenu(tr("&Tools"));
};

void QtImageViewer::openFile(){
	QFileDialog dialog(this, tr("Open File"));
	QString filename = dialog.getOpenFileName(this, "Select image to open");
	std::cout<<"Opening: "<<filename.toStdString()<<std::endl;

	TiffLoader* tiffLoader = new TiffLoader(filename.toStdString());
	tiffLoader->printMetaData();
	Image<>* myImage = tiffLoader->loadImage();

	showImageLeft(myImage);
	delete(myImage);
};

void QtImageViewer::showImage(Image<> *img, ImageView imageView){

	// get widgets for requested side
	QtCharts::QChartView **chartView{nullptr};
	QSplitter **splitter{nullptr};
	QLabel **imageLabel{nullptr};
	QScrollArea **scrollArea{nullptr};

	switch(imageView){

	case ImageView::LEFT:

		if(_imageState & ImageView::LEFT){
			std::cout<<"Error, already showing an image on left side"<<std::endl;
			return;
		}

		chartView = &_lChartView;
		splitter = &_leftSplitter;
		imageLabel = &_lImageLabel;
		scrollArea = &_lScrollArea;

		_imageState |= ImageView::LEFT;
		break;

	case ImageView::RIGHT:
		if(_imageState & ImageView::RIGHT){
			std::cout<<"Error, already showing an image on right side"<<std::endl;
			return;
		}

		chartView = &_rChartView;
		splitter = &_rightSplitter;
		imageLabel = &_rImageLabel;
		scrollArea = &_rScrollArea;

		_imageState |= ImageView::RIGHT;
		break;

	default:
		std::cout<<"showImage - error - image view not correctly specified: "<<imageView<<std::endl;
		break;
	}

	QImage::Format format = QImage::Format_Invalid;
	std::vector<unsigned int> hist{};

	if(img->getChannels() == 3){
		format = QImage::Format_RGB888;
		std::cout<<"Setting empty histogram for RGB image"<<std::endl;
		hist = std::vector<unsigned int>(256,0);
	}
	else if (img->getChannels() == 1){
		format = QImage::Format_Grayscale8;
		hist = img->getHistogram();
	}

	QtCharts::QBarSet *bset = new QtCharts::QBarSet("Intensities");
	unsigned int maxInt{0};
	for(unsigned int i : hist) { (*bset) << i; maxInt = std::max(maxInt,i);
		//std::cout<<i<<std::endl;
	}

	QtCharts::QBarSeries *bseries = new QtCharts::QBarSeries();
	bseries->append(bset);

	QtCharts::QChart *chart = new QtCharts::QChart();
	chart->addSeries(bseries);
	chart->setTitle("Intensity Histogram");
	chart->setAnimationOptions(QtCharts::QChart::NoAnimation);

	QtCharts::QValueAxis *xAxis = new  QtCharts::QValueAxis();
	xAxis->setRange(0,255);

	QtCharts::QValueAxis *yAxis = new QtCharts::QValueAxis();
	yAxis->setRange(0,maxInt);

	// Seems you must add axis to chart first, then to bseries
	chart->addAxis(xAxis, Qt::AlignBottom);
	chart->addAxis(yAxis, Qt::AlignLeft);

	bseries->attachAxis(xAxis);
	bseries->attachAxis(yAxis);
	bset->setBorderColor(QColor("black"));

	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignBottom);

	(*chartView) = new QtCharts::QChartView(chart);
	(*chartView)->setRenderHint(QPainter::Antialiasing);
	//  _chartView->setRenderHint(QPainter::TextAntialiasing);
	//_mainSplitter->setOrientation(Qt::Vertical);
	(*splitter)->addWidget(*chartView);

	// Copy image data to Qt
	std::cout<<"Width: "<<img->getWidth()
			 <<"\nHeight: "<<img->getHeight()
			 <<"\nBPC: "<<img->getBpc()
			 <<"\nBytes per line: "<<img->getWidth()*img->getChannels()*img->getBpc()/8<<std::endl;
		
	QImage qImg(img->getImageData(),
				img->getWidth(),
				img->getHeight(),
				img->getWidth()*img->getChannels()*img->getBpc()/8,
				format);

	// Tell Qt to show this image data
	(*imageLabel)->setPixmap(QPixmap::fromImage(qImg));
	(*imageLabel)->resize((*imageLabel)->pixmap()->size());
	(*scrollArea)->setVisible(true);

	update(); // For Qt to redraw with new image
};



void QtImageViewer::showImageLeft(Image<> *img) {
	showImage(img, ImageView::LEFT);
};

void QtImageViewer::showImageRight(Image<> *img) {
	showImage(img, ImageView::RIGHT);
};

void QtImageViewer::setCurrentImage(Image<uint8_t>* img){
	if(img){
		_currentImage = img;
	}
};

void QtImageViewer::clearFile(){};

void QtImageViewer::togreyscale(){
	ImageProcessor processor;

	QString filename = QFileDialog::getOpenFileName(this, "Select image for conversion");
	if(filename.isEmpty()){
		return;
	}
	TiffLoader trloader(filename.toStdString());
	Image<>* rgb = trloader.loadImage();
	Image<>* gsc = processor.togreyscale(rgb);
	showImageRight(gsc);
	delete gsc;
};

void QtImageViewer::combineImagesRGB() {
    ImageProcessor processor;

    QString redFile   = QFileDialog::getOpenFileName(this, "Select RED channel TIFF");
    if(redFile.isEmpty()) return;

    QString greenFile = QFileDialog::getOpenFileName(this, "Select GREEN channel TIFF");
    if(greenFile.isEmpty()) return;

    QString blueFile  = QFileDialog::getOpenFileName(this, "Select BLUE channel TIFF");
    if(blueFile.isEmpty()) return;

    TiffLoader redLoader(redFile.toStdString());
    Image<>* red = redLoader.loadImage();

    TiffLoader greenLoader(greenFile.toStdString());
    Image<>* green = greenLoader.loadImage();

    TiffLoader blueLoader(blueFile.toStdString());
    Image<>* blue = blueLoader.loadImage();

    if(!red || !green || !blue) {
        std::cout << "Failed to load one of the images." << std::endl;
        return;
    }

    Image<>* rgb = processor.combineRGB(red, green, blue);

    if(!rgb) {
        std::cout << "RGB combination failed." << std::endl;
        return;
    }

    showImageRight(rgb);
	QString filename = QFileDialog::getSaveFileName(this, "Save as...");
	if(!filename.isEmpty()) {
		QImage image (
			rgb->getImageData(),
			rgb->getWidth(),
			rgb->getHeight(),
			rgb->getWidth() * 3,
			QImage::Format_RGB888
		);
		QImage copy = image.copy();
		copy.save(filename);
	}
  
    delete red;
    delete green;
    delete blue;
	delete rgb;
};

void QtImageViewer::negate() {
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select Image for transformation...");
    if(filename.isEmpty()) return;
	
	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.negationtr(img);
	showImageRight(img);
	delete img; // make this work somehow with qt getting ownership of the image. Maybe also merge functions somehow?
};

void QtImageViewer::negateLUT() {
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select Image for transformation...");
    if(filename.isEmpty()) return;
	
	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.negationlut(img);
	showImageRight(img);

	delete img;
};

void QtImageViewer::powerlaw(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for transformation...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.powerlawtr(img);
	showImageRight(img);

	delete img;
};

void QtImageViewer::powerlawLUT(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for transformation...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.powerlawlut(img);
	showImageRight(img);

	delete img;
};

void QtImageViewer::linear(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for transformation...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.lineartr(img);
	showImageRight(img);
	delete img;
};

void QtImageViewer::linearLUT(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for transformation...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.linearlut(img);
	showImageRight(img);
	delete img;
};

void QtImageViewer::thresholding(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for thresholding...");
	if(filename.isEmpty()) return;
	
	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.thresholdtr(img);
	showImageRight(img);
	delete img;
};

void QtImageViewer::thresholdingLUT(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for thresholding...");
	if(filename.isEmpty()) return;
	
	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.thresholdlut(img);
	showImageRight(img);
	delete img;
};

void QtImageViewer::histogrameq(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for equalization...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	processor.histogramtr(img);
	showImageRight(img);
	delete img;
};

void QtImageViewer::lowpass(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for lowpass filtering...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	Image<>* filtered = processor.lowpass(img);
	showImageRight(filtered);
	delete img;
	delete filtered;
};
void QtImageViewer::median(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for median filtering...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	Image<>* filtered = processor.median(img);
	showImageRight(filtered);
	delete img;
	delete filtered;
};
void QtImageViewer::laplacian(){
	ImageProcessor processor;
	QString filename = QFileDialog::getOpenFileName(this, "Select image for laplacian transform...");
	if(filename.isEmpty()) return;

	TiffLoader trloader = (filename.toStdString());
	Image<>* img = trloader.loadImage();

	Image<>* filtered = processor.laplacian(img);
	showImageRight(filtered);
	delete img;
	delete filtered;
};

void QtImageViewer::quit(){
	close();
};