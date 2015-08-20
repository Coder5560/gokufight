//#include "Dialog.h"
//
//
//Dialog::Dialog(){}
//
//Dialog::~Dialog(){}
//
//void Dialog::setSize(Size size){
//	if (layout){
//		layout->setContentSize(size);
//		layout->setBackGroundImageCapInsets(Rect(4,4,size.width-8,size.height-8));
//	}
//
//}
//void Dialog::setBackground(std::string backgroundUrl){
//	if (layout){
//		layout->setBackGroundImage(backgroundUrl,ui::Widget::TextureResType::LOCAL);
//		layout->setBackGroundImageScale9Enabled(true);
//		layout->setBackGroundImageCapInsets(Rect(4, 4, layout->getContentSize().width - 8, layout->getContentSize().height - 8));
//	}
//}
//
//void Dialog::setNegativeButton(std::string textNegative,const std::function<void()> &callback){
//	this->negativeCallback = callback;
//	float width = layout->getContentSize().width / 2;
//	float height = (layout->getContentSize().height > 400) ? 120 : 80;
//	layoutNegative = ui::Layout::create();
//	layoutNegative->setLayoutType(ui::Layout::Type::ABSOLUTE);
//	
//
//	ui::Button* buttonNegative = ui::Button::create();
//	buttonNegative->setScale9Enabled(true);
//	buttonNegative->setTitleText(textNegative);
//	buttonNegative->setTitleColor(Color3B::BLACK);
//	buttonNegative->setTitleFontName("fonts/courbd.ttf");
//	buttonNegative->setTitleFontSize(20);
//	buttonNegative->setContentSize(Size(width,height));
//	
//
//}
//void Dialog::build(){
//
//
//}