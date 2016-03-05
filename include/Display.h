#pragma once

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <string>
#include <vector>
#include <thread>
#include <atomic>

class Display
{
public:
	Display();
	~Display();

	void init();
	void run();

private:
	//View Moving code
	void moveView();

	//Event handler
	void eventHandle();

	//Thread for video capture
	void captureThread(cv::Mat *src, cv::VideoCapture *cap);

	//Process Video into a texture
	void processVideo(cv::Mat src, sf::Texture &texture);

	//Setup Gui Enviroment
	void createGui();

	void smoothOut(){ m_isSmooth = !m_isSmooth; m_texture.setSmooth(m_isSmooth); m_sprite.setTexture(m_texture); }
	void setFrame(){ m_src.copyTo(m_setFrame); }
	bool m_isSmooth = false;

	//Gui Object
	sfg::SFGUI m_sfgui;

	//SFGui varibles
	sfg::Window::Ptr m_window;
	sfg::Button::Ptr m_defaultButton;
	sfg::Scale::Ptr m_thresh;
	sfg::Label::Ptr m_slideLabel;

	//Capture Thread
	std::thread m_captureThread;

	//Capture object and image
	cv::VideoCapture m_cap;
	cv::Mat m_src;
	cv::Mat m_setFrame;

	//SFML varibles
	sf::RenderWindow m_renderWindow;
	sf::Clock m_clock;
	sf::View m_view;
	sf::Event m_event;
	float m_zoom = 1.0f;

	//Iamge, Sprite and Texture Objects
	sf::Image m_image;
	sf::Sprite m_sprite;
	sf::Texture m_texture;


};

