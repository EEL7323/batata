// 
// 
// 

#include "captcha.h"


captcha::captcha()
{
	randomSeed(millis());
	return;
}
captcha::~captcha()
{

	return;
}
String captcha::newCaptcha()
{
	curr_captcha = String(random(0, 99999));
	return curr_captcha; 
}
bool captcha::checkCaptcha(String message)
{
	if (message == curr_captcha) return true;
	else return false;
}
