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
}

QtImageViewer::~QtImageViewer(){
	delete(_lImageLabel);
	delete(_lScrollArea);
	delete(_rImageLabel);
	delete(_rScrollArea);
	delete(_fileMenu);
	delete(_fileOpenAction);
	delete(_fileClearAction);
	delete(_quitAction);
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

	_toolsFourierTransform = new QAction(tr("&Fourier transform..."), this);
	_toolsFourierTransform->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));

	_toolsCombine = new QAction(tr("&Combine images into RGB..."), this);
	_toolsCombine->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));
	connect(_toolsCombine, SIGNAL(triggered()), this, SLOT(combineImagesRGB()));

	_fileMenu = menuBar()->addMenu(tr("&File"));
	_fileMenu->addAction(_fileOpenAction);
	_fileMenu->addAction(_fileClearAction);
	_fileMenu->addAction(_quitAction);

	_toolsMenu = menuBar()->addMenu(tr("&Tools"));
	_toolsMenu->addAction(_toolsFourierTransform);
	_toolsMenu->addAction(_toolsCombine);
}

void QtImageViewer::openFile(){
	QFileDialog dialog(this, tr("Open File"));
	QString filename = dialog.getOpenFileName(this, "Select image to open");
	std::cout<<"Opening: "<<filename.toStdString()<<std::endl;

	TiffLoader* tiffLoader = new TiffLoader(filename.toStdString());
	tiffLoader->printMetaData();
	Image* myImage = tiffLoader->loadImage();

	showImageLeft(myImage);
	delete(myImage);
}

void QtImageViewer::showImage(Image *img, ImageView imageView){

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
}



void QtImageViewer::showImageLeft(Image *img) {
	showImage(img, ImageView::LEFT);
}

void QtImageViewer::showImageRight(Image *img) {
	showImage(img, ImageView::RIGHT);
}

void QtImageViewer::clearFile() {

	_lImageLabel->clear();
	_lScrollArea->setVisible(false);

	_rImageLabel->clear();
	_rScrollArea->setVisible(false);
	_imageState = 0;

	update();
}


void QtImageViewer::combineImagesRGB() {
    ImageProcessor processor;

    QString redFile   = QFileDialog::getOpenFileName(this, "Select RED channel TIFF");
    if(redFile.isEmpty()) return;

    QString greenFile = QFileDialog::getOpenFileName(this, "Select GREEN channel TIFF");
    if(greenFile.isEmpty()) return;

    QString blueFile  = QFileDialog::getOpenFileName(this, "Select BLUE channel TIFF");
    if(blueFile.isEmpty()) return;

    TiffLoader redLoader(redFile.toStdString());
    Image* red = redLoader.loadImage();

    TiffLoader greenLoader(greenFile.toStdString());
    Image* green = greenLoader.loadImage();

    TiffLoader blueLoader(blueFile.toStdString());
    Image* blue = blueLoader.loadImage();

    if(!red || !green || !blue) {
        std::cout << "Failed to load one of the images." << std::endl;
        return;
    }

    Image* rgb = processor.combineRGB(red, green, blue);

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
}



void QtImageViewer::quit(){
	close();
};
