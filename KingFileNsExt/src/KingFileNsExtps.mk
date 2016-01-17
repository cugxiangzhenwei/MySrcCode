
SimpleNsExtps.dll: dlldata.obj SimpleNsExt_p.obj SimpleNsExt_i.obj
	link /dll /out:SimpleNsExtps.dll /def:SimpleNsExtps.def /entry:DllMain dlldata.obj SimpleNsExt_p.obj SimpleNsExt_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del SimpleNsExtps.dll
	@del SimpleNsExtps.lib
	@del SimpleNsExtps.exp
	@del dlldata.obj
	@del SimpleNsExt_p.obj
	@del SimpleNsExt_i.obj
