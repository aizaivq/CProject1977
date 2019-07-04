#include <OpenGL.h>
class Camera{
private:
	int mFd;
	int mFrameFd;
	int mWidth;
	int mHeight;
	int mFormat;
	int FrameSave(const char *path);
public:

	int Init(int id,int width,int height,int format);
	int StartPreview();
	int StartPreviewInner();


};