#ifndef _GO_VIDEO_GRABBER
#define _GO_VIDEO_GRABBER

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofGraphics.h"
#include "ofTypes.h"

#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	#include "goQtUtils.h"
#endif

#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	#include "videoInput.h"
    #include <map>
/*
    #define OF_CAMERACONTROL_FLAGS_AUTO        0X0001L
    #define OF_CAMERACONTROL_FLAGS_MANUAL      0X0002L

    #define OF_CAMERACONTROL_FLAGS_ABSOLUTE    0X0000L
    #define OF_CAMERACONTROL_FLAGS_RELATIVE    0X0010L*/

	class setting {
        public:

            string  propName;
            long    propID;
            long    min;
            long    max;
            long    SteppingDelta;
            long    CurrentValue;
            long    DefaultValue;
            long    flags;
            float   pctValue;

            bool valToPct() {

                float range = (float)max - (float)min;
                if(range <= 0)return false;
                if(SteppingDelta == 0) return false;

                pctValue = (float)(CurrentValue - min ) / range;

                return true;
            }

            string  print()
            {
                valToPct();
                string settingsString = propName              +
                                        " id: " + ofToString((int)propID) +
                                        " min: " + ofToString((int)min) +
                                        " max: " + ofToString((int)max) +
                                        " SteppingDelta: " + ofToString((int)SteppingDelta) +
                                        " currentValue: " + ofToString((int)CurrentValue) +
                                        " pctValue: " + ofToString((float)pctValue) +
                                        " defaultValue: " + ofToString((int)DefaultValue) +
                                        " flags: " + ofToString((int)flags);
                return settingsString;
            }
	};

#endif

#ifdef OF_VIDEO_CAPTURE_UNICAP
	#include "ofUCUtils.h"
#endif

#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	#include "ofGstUtils.h"
#endif

// todo:
// 		QT - callback, via SGSetDataProc - couldn't get this to work yet
// 		image decompress options ala mung...


class goVideoGrabber : public ofBaseVideo{

	public :

		goVideoGrabber();
		virtual ~goVideoGrabber();

		void 			listDevices();
		bool 			isFrameNew();
		void			grabFrame();
		void			close();
		bool			initGrabber(int w, int h, bool bTexture = true);
		void			videoSettings();
		unsigned char 	* getPixels();
		ofTexture &		getTextureReference();
		void 			setVerbose(bool bTalkToMe);
		void			setDeviceID(int _deviceID);
#ifdef OF_VIDEO_CAPTURE_QUICKTIME
		void            setDeviceID(string _deviceID);
#endif

#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
        void                    showSettingsWindow();
        void                    setCameraSettings(map<string, setting> settings);
        void                    setCameraSetting(setting s);
        map<string, setting>    getCameraSettings();
        setting                 getCameraSetting(long propID, string propName);
        void                    setFilterSettings(map<string, setting> settings);
        void                    setFilterSetting(setting s);
        map<string, setting>    getFilterSettings();
        setting                 getFilterSetting(long propID, string propName);
#endif

		void			setDesiredFrameRate(int framerate);
		void 			setUseTexture(bool bUse);
		void 			draw(float x, float y, float w, float h);
		void 			draw(float x, float y);
		void			update();


		//the anchor is the point the image is drawn around.
		//this can be useful if you want to rotate an image around a particular point.
        void			setAnchorPercent(float xPct, float yPct);	//set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
        void			setAnchorPoint(int x, int y);				//set the anchor point in pixels
        void			resetAnchor();								//resets the anchor to (0, 0)

		float 			getHeight();
		float 			getWidth();

		int			height;
		int			width;

	protected:

		bool					bChooseDevice;
		int						deviceID;
		bool					bUseTexture;
		ofTexture 				tex;
		bool 					bVerbose;
		bool 					bGrabberInited;
	    unsigned char * 		pixels;
		int						attemptFramerate;
		bool 					bIsFrameNew;

		//--------------------------------- quicktime
		#ifdef OF_VIDEO_CAPTURE_QUICKTIME
			vector<string>			deviceVec;

			unsigned char * 	offscreenGWorldPixels;	// 32 bit: argb (qt k32ARGBPixelFormat)
			int 				w,h;
			bool 				bHavePixelsChanged;
			GWorldPtr 			videogworld;
			SeqGrabComponent	gSeqGrabber;
			SGChannel 			gVideoChannel;
			Rect				videoRect;
			bool 				bSgInited;
			string				deviceName;
			SGGrabCompleteBottleUPP	myGrabCompleteProc;

			bool				qtInitSeqGrabber();
			bool				qtCloseSeqGrabber();
			bool				qtSelectDevice(int deviceNumber, bool didWeChooseADevice);

			//--------------------------------------------------------------------
			#ifdef TARGET_OSX
			//--------------------------------------------------------------------

			bool					saveSettings();
			bool					loadSettings();

			//--------------------------------------------------------------------
			#endif
			//--------------------------------------------------------------------


		#endif

		//--------------------------------- directshow
		#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
			int 					device;
			videoInput 				VI;

			bool 					bDoWeNeedToResize;

		#endif

		//--------------------------------- linux unicap
		#ifdef OF_VIDEO_CAPTURE_UNICAP
			ofUCUtils				ucGrabber;
		#endif

		#ifdef OF_VIDEO_CAPTURE_GSTREAMER
			ofGstUtils				gstUtils;
		#endif

};




#endif

