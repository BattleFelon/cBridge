#include "Display.h"

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <vector>
#include <thread>
#include <atomic>
#include <iostream>

Display::Display()
{
}


Display::~Display()
{
}

void Display::init()
{
	//Set Window and View varibles
	m_renderWindow.create(sf::VideoMode(800, 600), "Bridge App");
	m_view = m_renderWindow.getDefaultView();
	m_renderWindow.setFramerateLimit(60);

	//Open webcam
	m_cap.open(0);

	while (!m_cap.isOpened() && !m_cap.grab()){
		std::cout << "Not Open Yet" << std::endl;
	}

	//Start capture thread
	m_captureThread = std::thread(&Display::captureThread, this, &m_src, &m_cap);

	//Checks to see src is not empty
	int i = 0;
	while (m_src.empty()){};

	//Create texture and set it to the sprite
	processVideo(m_src, m_texture);

	m_sprite.setTexture(m_texture);

	m_sprite.setOrigin((float)m_sprite.getScale().x * m_sprite.getLocalBounds().width / 2.0f,
(float)m_sprite.getScale().y * m_sprite.getLocalBounds().height / 2.0f);
	m_sprite.setPosition(m_renderWindow.getSize().x / 2.f, m_renderWindow.getSize().y / 2.f);

	createGui();
}

void Display::run()
{

	while (m_renderWindow.isOpen())
	{

		//Check setFrame to process and highlight changes
		m_slideLabel->SetText(std::to_string(m_thresh->GetValue()));
		if (!m_setFrame.empty()){
			cv::Mat diff, src;
			m_src.copyTo(src);

			cv::subtract(m_setFrame, m_src, diff);
			cv::cvtColor(diff, diff, cv::COLOR_BGR2GRAY);

			for (int i = 0, n = diff.rows; i < n; ++i){
				for (int j = 0, m = diff.cols; j < m; ++j){
					if (diff.at<uchar>(i, j) > m_thresh->GetValue())
						src.at<cv::Vec3b>(i, j)(2) < 100 ? src.at<cv::Vec3b>(i, j)(2) = src.at<cv::Vec3b>(i, j)(2) + 150 : 0 ;
				}
			}
			processVideo(src, m_texture);
		}
		else{
			//Set texture
			processVideo(m_src, m_texture);
		}

		moveView();
		eventHandle();

		//Update GUI
		m_window->Update(0.0f);

		//Set view
		m_renderWindow.setView(m_view);

		//Clear previous frame
		m_renderWindow.clear(sf::Color::Blue);


		m_renderWindow.draw(m_sprite);

		//Show render windows
		m_sfgui.Display(m_renderWindow);
		m_renderWindow.display();
	}
	m_captureThread.join();
}

void Display::moveView()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_view.move(sf::Vector2f(0.f, -20.0f*m_zoom));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_view.move(sf::Vector2f(0.f, 20.0f * m_zoom));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_view.move(sf::Vector2f(-20.f * m_zoom, 0.f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_view.move(sf::Vector2f(20.f * m_zoom, 0.f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		m_view.rotate(2.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		m_view.rotate(-2.0f);
	}
}

void Display::eventHandle()
{
	while (m_renderWindow.pollEvent(m_event))
	{
		//Gui Event Handler
		m_window->HandleEvent(m_event);

		// "close requested" event: we close the m_renderWindow
		if (m_event.type == sf::Event::Closed)
			m_renderWindow.close();

		// catch the resize events
		if (m_event.type == sf::Event::Resized)
		{
			// update the view to the new size of the window
			sf::FloatRect visibleArea(0, 0, (float)m_event.size.width, (float)m_event.size.height);
			m_renderWindow.setView(sf::View(visibleArea));
			m_view = m_renderWindow.getView();
		}

		//Wheel Moving Listener
		if (m_event.type == sf::Event::MouseWheelMoved)
		{

			if (m_event.mouseWheel.delta > 0)
			{
				m_zoom *= .90f;
				m_view.zoom(.9f);
			}
			else if (m_event.mouseWheel.delta < 0)
			{
				m_zoom /= .90f;
				m_view.zoom(1.f / .9f);
			}
		}
	}
}


void Display::captureThread(cv::Mat *src, cv::VideoCapture *cap)
{
	while (m_renderWindow.isOpen()){
		if (cap->grab()){
			(*cap) >> (*src);
		}
		else
		{
			std::cout << "No Video Data" << '\n';
		}
	}
	m_cap.release();
}

void Display::processVideo(cv::Mat src, sf::Texture &texture)
{
	cv::Mat srcConvert;
	if (src.empty())
		std::cout << "Empty Image" << std::endl;
	else
	{
		cv::cvtColor(src, srcConvert, cv::COLOR_BGR2RGBA);
		m_image.create(src.cols, src.rows, srcConvert.ptr());
		texture.loadFromImage(m_image);
	}
}

void Display::createGui()
{
	//Create Settings Window 
	m_window = sfg::Window::Create();
	m_window->SetTitle("Settings");

	//Create box and pack button
	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);

	m_defaultButton = sfg::Button::Create("Smooth");
	auto frameSet = sfg::Button::Create("Set Frame");
	m_slideLabel = sfg::Label::Create("NA");

	m_thresh = sfg::Scale::Create(sfg::Scale::Orientation::HORIZONTAL);
	m_thresh->SetRange(5.f, 50.f);
	m_thresh->SetIncrements(3.f, 12.f);
	m_thresh->SetValue(10.f);
	m_thresh->SetRequisition(sf::Vector2f(0.f, 100.f));

	m_defaultButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&Display::smoothOut, this));
	frameSet->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&Display::setFrame,this));

	box->Pack(m_defaultButton);
	box->Pack(frameSet);
	box->Pack(m_slideLabel);
	box->Pack(m_thresh);

	//Add box to window
	m_window->Add(box);
}
