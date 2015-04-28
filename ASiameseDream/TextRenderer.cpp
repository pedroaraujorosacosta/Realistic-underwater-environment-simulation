#include "TextRenderer.h"
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>

template<> TextRenderer* Ogre::Singleton<TextRenderer>::msSingleton = 0;

TextRenderer::TextRenderer()
{
	_overlayMgr = Ogre::OverlayManager::getSingletonPtr();

	_overlay = _overlayMgr->create("overlay1");
	_panel = static_cast<Ogre::OverlayContainer*>(_overlayMgr->createOverlayElement("Panel", "container1"));
	_panel->setDimensions(1, 1);
	_panel->setPosition(0, 0);
	_overlay->add2D(_panel);



	_overlay->show();

	/*_panel->setMetricsMode(Ogre::GMM_PIXELS);
	_panel->setPosition(10, 10);
	_panel->setDimensions(100, 100);
	Ogre::TextAreaOverlayElement* textArea = static_cast<Ogre::TextAreaOverlayElement*>(
		_overlayMgr->createOverlayElement("TextArea", "TextAreaName"));
	textArea->setMetricsMode(Ogre::GMM_PIXELS);
	textArea->setPosition(0, 0);
	textArea->setDimensions(100, 100);
	textArea->setCaption("Hello, World!");
	textArea->setCharHeight(16);
	textArea->setFontName("MyFont");
	textArea->setColourBottom(Ogre::ColourValue(0.3, 0.5, 0.3));
	textArea->setColourTop(Ogre::ColourValue(0.5, 0.7, 0.5));



	_panel->addChild(textArea);*/
}

void TextRenderer::addTextBox(const std::string& ID,
	const std::string& text,
	Ogre::Real x, Ogre::Real y,
	Ogre::Real width, Ogre::Real height,
	const Ogre::ColourValue& color)
{
	/*Ogre::TextAreaOverlayElement* textBox = static_cast<Ogre::TextAreaOverlayElement*>(_overlayMgr->createOverlayElement("TextArea", ID));
	textBox->setMetricsMode(Ogre::GMM_PIXELS);
	textBox->setPosition(x, y);
	textBox->setDimensions(width, height);
	//textBox->setWidth(width);
	//textBox->setHeight(height);
	textBox->setCaption(text);
	textBox->setCharHeight(16);
	textBox->setFontName("MyFont");
	//textBox->setParameter("font_name", "MyFont");
	//textBox->setParameter("char_height", "0.3");
	//textBox->setColour(color);
	textBox->setColourBottom(Ogre::ColourValue(0.3, 0.5, 0.3));
	textBox->setColourTop(Ogre::ColourValue(0.5, 0.7, 0.5));


	_panel->addChild(textBox);*/

	Ogre::TextAreaOverlayElement* textArea = static_cast<Ogre::TextAreaOverlayElement*>(
		_overlayMgr->createOverlayElement("TextArea", "TextAreaName"));
	textArea->setMetricsMode(Ogre::GMM_PIXELS);
	textArea->setPosition(0, 0);
	textArea->setDimensions(100, 100);
	textArea->setCaption("Hello, World!");
	textArea->setCharHeight(16);
	textArea->setFontName("MyFont");
	textArea->setColourBottom(Ogre::ColourValue(0.3, 0.5, 0.3));
	textArea->setColourTop(Ogre::ColourValue(0.5, 0.7, 0.5));



	_panel->addChild(textArea);
}

void TextRenderer::removeTextBox(const std::string& ID)
{
	_panel->removeChild(ID);
	_overlayMgr->destroyOverlayElement(ID);
}

void TextRenderer::setText(const std::string& ID, const std::string& Text)
{
	Ogre::OverlayElement* textBox = _overlayMgr->getOverlayElement(ID);
	textBox->setCaption(Text);
}

const std::string& TextRenderer::getText(const std::string& ID)
{
	Ogre::OverlayElement* textBox = _overlayMgr->getOverlayElement(ID);
	return textBox->getCaption();
}