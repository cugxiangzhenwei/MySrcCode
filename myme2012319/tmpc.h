
/*
 *	main of tmpc
 */

#ifndef _TMPC_H_
#define _TMPC_H_

#ifdef __TMPC_WIN32__
	#ifdef TMPC_EXPORTS
		#define TMPC_API  __declspec(dllexport)
	#else
		#define TMPC_API  __declspec(dllimport)
	#endif
#elif defined __TMPC_LINUX_ARM__
	#define TMPC_API
	#define WINAPI
#elif defined __TMPC_SYMBIAN__
#define TMPC_API IMPORT_C
#define WINAPI
#endif

#define VERSION_MAX_LENGTH 32

/*
#ifndef __TMPC_LINUX_ARM__
#define int64_t __int64
#else
#define int64_t signed long long 
#endif
*/

#define		_BASE							0x0000
#define		_PLAY							0x00FF
#define		_PLUGIN							0xFF00

#define		TMPC_OK							_BASE+1
#define		TMPC_OUT_OF_MEMORY				_BASE+2
#define		TMPC_INIT_AUDIO_ERROR			_BASE+3
#define		TMPC_OPEN_AUDIO_ERROR			_BASE+4
#define		TMPC_INIT_VIDEO_ERROR			_BASE+5
#define		TMPC_OPEN_VIDEO_ERROR			_BASE+6
#define		TMPC_CREATE_264_DECODE_ERROR	_BASE+7

#define		TMPC_NOTIFY_MEDIA_INFO			_PLAY+0
#define		TMPC_START_BUFFER_DATA			_PLAY+1
#define		TMPC_CONNECT_SERVER_OK			_PLAY+2
#define		TMPC_DISCONNECT_FROM_SERVER		_PLAY+3
#define		TMPC_DATA_DOWNLOAD_FINISH		_PLAY+4
#define		TMPC_START_PLAY					_PLAY+5
#define		TMPC_INITIAL_STREAM_ERROR		_PLAY+6
#define		TMPC_WAIT_AUDIO_DATA			_PLAY+7
#define		TMPC_WAIT_VIDEO_DATA			_PLAY+8
#define		TMPC_PLAY_FINISH				_PLAY+9
#define		TMPC_NETWORK_ERROR				_PLAY+10
#define		TMPC_MEDIA_SPEC_ERROR			_PLAY+11
#define		TMPC_PRE_PLAY					_PLAY+12
#define		TMPC_NET_RECONNECT				_PLAY+13

#define		TMPC_NO_AUDIO_CODEC				_PLUGIN+0
#define		TMPC_NO_VIDEO_CODEC				_PLUGIN+1
#define		TMPC_NO_PLAY_OBJECT				_PLUGIN+2
#define		TMPC_LOAD_PLUGIN_FAILED			_PLUGIN+3
#define		TMPC_NOT_IMPLEMENT_PLUGIN		_PLUGIN+4
#define		TMPC_OPEN_PLUGIN_FAILED			_PLUGIN+5
#define		TMPC_START_PLUGIN_FAILED		_PLUGIN+6
#define		TMPC_RECONNECT_TO_SERVER		_PLUGIN+7
#define		TMPC_CHARGE_ACCESS_REFUSAL		_PLUGIN+9
#define		TMPC_TEMOBI_ACCESS_REFUSAL		_PLUGIN+10
#define		TMPC_TEMOBI_TIME_OUT			_PLUGIN+11

/*play state*/
#define		TMPC_PS_UNKNOWN			0x00
#define 	TMPC_PS_CONNECT			0x01
#define 	TMPC_PS_STOP			0x02
#define 	TMPC_PS_PLAY			0x03
#define 	TMPC_PS_PLAY_PAUSE		0x04
#define		TMPC_PS_BUFFER			0x05
#define 	TMPC_PS_SEEKING			0x06
#define 	TMPC_PS_BUFFER_PAUSE	0x07
#define		TMPC_PS_BKG_MODE		0x08

/*picture rotate*/
#define		IMG_DISPLAY_NORMAL		0x00
#define		IMG_DISPLAY_RIGHT_90	0x01
/*
 *	plugin message ID
 */

#define		TMPC_RUN_MODE_NORMAL		0
#define		TMPC_RUN_MODE_REAL_TIME		1

/*network*/

/*access point*/
#define TMPC_NETWORK_CMNET	0
#define TMPC_NETWORK_CMWAP	1

/*link type,such as TCP, UDP, TCP proxy*/
#define TMPC_LINK_TYPE_UDP				0
#define TMPC_LINK_TYPE_TCP				1
#define TMPC_LINK_TYPE_TCP_USEPROXY		2


#define TMPC_ENV_SH_VERSION		0
#define TMPC_ENV_GD_VERSION		1

#define TMPC_REC_ONLY_AUDIO		1
#define TMPC_REC_ONLY_VIDEO		2
#define TMPC_REC_VIDEO_AUDIO	3


#ifdef __cplusplus
extern "C" {
#endif

	/*
	 * callback function prototype is following:
	 * void (*notify_call_back)( void *user,int notify_id, unsigned char *stream, unsigned int len )
	 */
	typedef void (WINAPI *NOTIFY_CALLBACK)( void *user,int notify_id, unsigned int param );
	typedef void (WINAPI *RECORD_CALLBACK)( void *user,unsigned char *buf, int len, int category, char flag);

    // 统计点击量结构体--客户端
    typedef struct tagCDR_CLIENT
    {
        unsigned int CHANNEL_ID;   // 栏目ID
        unsigned int PROG_ID;      // 节目ID
        char		BILL_ID[12];        // 计费标识
        char		Served_msisdn[32];  // 手机用户的唯一MSISDN
        char		Reserved[12];        // 预留以后扩充，凑足64 Bytes
    }CDR_CLIENT;

	typedef struct customer_info
	{
		char *login_name;
		int   password_len;//登陆用户的密码长度
		char *password;
		int   content_id;//节目内容ID
		char *icp_id;//icpid
		char *charge_URL;//华为原始的url,这个比较长注意缓冲区长度，最好在2000个字节左右
	}customer_info;

	typedef struct player_parameter
	{
		char		play_url[256];
		char		host_addr[32];
		short		hostport;  
		short		guestport; /*optional*/
		void		*handle;
#ifdef __TMPC_SYMBIAN__
		void        *iWsSession;
		void        *iScreen;
#endif
		int				av_flag;	/*Audio,Video receive flag*/
		int				start_pos;
		
		unsigned int    should_buffer_time; /*unit is millisecond*/
		unsigned int    xpos; 
		unsigned int    ypos; 
		char			cur_dir_for_linux[256];

		//guangdong
		CDR_CLIENT		cdr_parm;

		//shanghai
		customer_info	Fee_Info;
		char			authorization_code[32];
		char			timestamp[32];

		/*new item about network*/
		int		APN_Type; /*TMPC_NETWORK_CMNET,TMPC_NETWORK_CMWAP*/
		int		APN_ID;   /*0,1,2...*/
		int		Link_Type;/*TMPC_LINK_TYPE_UDP,TMPC_LINK_TYPE_TCP,TMPC_LINK_TYPE_TCP_USEPROXY*/

		/*only for TMPC_LINK_TYPE_TCP_USEPROXY*/
		char	proxyaddr[32]; /*proxy server's address*/
		int		proxyport; /*proxy server's port*/

		int		use_env;/*must be TMPC_ENV_SH_VERSION OR TMPC_ENV_GD_VERSION*/
		int		display_outside;/*0:1*/

#ifdef __TMPC_WITH_PARTNER_ID__
		//partner ID
		char safecode[32];
		int safecodelen;
#endif//#ifdef __TMPC_WITH_PARTNER_ID__

	}player_parameter;

	typedef struct media_info
	{
		/*media length only for VOD*/
		unsigned int   dwDuration;/*unit is millisecond*/

		/*video*/
		unsigned int   videoWidth;
		unsigned int   videoHeight;
		char sz_video_name[32];
		int	video_available; /*1 or 0*/

		/*audio*/
		int	freq;		/* DSP frequency -- samples per second */
		int	format;		/* Audio data format,if every sample is 16bits, the value is 16. */
		int channels;	/* Number of channels: 1 mono, 2 stereo */
		char sz_audio_name[32];
		int	audio_available; /*1 or 0 */
	}media_info;

	typedef struct stream_pack
	{
		unsigned char *media;
		int	ts_len;
		unsigned char *data_buf;
		int data_len;
		unsigned int nType;
	}stream_pack;

	typedef struct player_status
	{
		/*video*/
		unsigned int delivered;
		unsigned int skipped;
		float	fps;

		/*buffer*/
		unsigned int video_kbps;
		unsigned int audio_kbps;

		unsigned int video_pkt_left;
		unsigned int audio_pkt_left;

		unsigned int video_pkt_gotten;
		unsigned int audio_pkt_gotten;

		unsigned int already_buffer_time;/*unit is millisecond*/
		unsigned int cur_play_pos;/*unit is millisecond*/
		int	cur_cpu_load; /*only for wince*/
	}player_status;

	/*main function*/
	
	/*only using it at first*/
	TMPC_API void*  tmpc_init(NOTIFY_CALLBACK func, void *user_data, const char *linux_dir );

	/*a play session*/
	TMPC_API void tmpc_configure_memory(void *pc, int min, int max ); /*unit is ms*/
	
	/*network flag is TMPC_NETWORK_CMNET or TMPC_NETWORK_CMWAP*/	
	TMPC_API void tmpc_start(void *pc,player_parameter*pp);
	TMPC_API void tmpc_record_callback(void *pc,RECORD_CALLBACK func, void *key);

	TMPC_API void tmpc_run_mode(void *pc,int mode, int max_tolerance_ms );
	TMPC_API void tmpc_set_playspeed( void *pc,int num, int den );
	TMPC_API int tmpc_seek( void *pc,int ms ); 
	TMPC_API void tmpc_pause(void *pc); 
	TMPC_API int  tmpc_resize(void *pc,int width, int height, unsigned int xpos, unsigned int ypos); //if succeed return 1
	TMPC_API void tmpc_get_media_info(void *pc, media_info* mi);
	TMPC_API int  tmpc_get_player_state(void *pc);
	TMPC_API void tmpc_get_status(void *pc,player_status*ps);
	TMPC_API void tmpc_snapshot(void *pc, char* save_url);
	TMPC_API void tmpc_stop(void *pc);
	TMPC_API int tmpc_set_audio_volume(void *pc,int val );
	TMPC_API int tmpc_get_audio_volume(void *pc);
//	TMPC_API void tmpc_changechannel(void *pc,char* addr, int port, char* url, int pos);
	TMPC_API int  tmpc_rotate(void *pc, int flag, int b_zoom, int w, int h);
	TMPC_API void tmpc_set_buffer_limit(void*pc, int min, int max ); /*unit is ms*/
	TMPC_API int  tmpc_memset_audio(void *pc, int val );
	TMPC_API int  tmpc_change_av_flag(void *pc, int val);

#ifdef __TMPC_LINUX_ARM__
	void tmpc_show_image(void *pc );
#endif 
	TMPC_API char* tmpc_get_player_version(void *pc); /*for example:0.1.1.1*/
	TMPC_API int   tmpc_get_versions(void *pc, char versions[][VERSION_MAX_LENGTH], int count);
	TMPC_API char* tmpc_get_network_version(void *pc); /*for example:0.1.1.1*/

	/*just one time*/
	TMPC_API void tmpc_quit(void*pc);

	TMPC_API int  tmpc_begin_show(void *pc);
	TMPC_API int  tmpc_get_raw_picture(void *pc, unsigned char *buf, int pixelFormat);
	TMPC_API int  tmpc_do_display(void *pc);
	TMPC_API int  tmpc_end_show(void *pc);
	/*for plugin */
	/*nType as following:TM_MEDIA_AUDIO,TM_MEDIA_VIDEO,TM_MEDIA_TITLE*/
	TMPC_API int FillSample(void *pc_handle, stream_pack *sp );

	TMPC_API void tmpc_display_property(void *pc_handle, int show);
	
	TMPC_API int  tmpc_close_audio_device(void *pc_handle);
	TMPC_API int  tmpc_open_audio_device(void *pc_handle);	

	TMPC_API int  tmpc_start2(void *pc_handle);
	TMPC_API int  tmpc_stop2(void *pc_handle);	
	
#ifdef __cplusplus
	}
#endif /*__cplusplus*/

#endif /*_TMPC_H_*/
