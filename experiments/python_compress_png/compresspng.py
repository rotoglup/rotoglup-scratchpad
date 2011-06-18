# simple PNG compression function, based on alex evans code (see EOF) from https://gist.github.com/908299

import ctypes
import socket
import zlib

_M32 = 0xffffffffL
def _htonl(n):
  # work around signedness issues
  return socket.htonl(n & _M32) & _M32

def compressPNG(img, w, h, numchans):
  
  p = w*numchans
  z = zlib.compressobj()
  zbuf =""
  for y in xrange(h):
    zbuf += z.compress("\0")
    zbuf += z.compress(img[y*p:y*p+p])
  zbuf += z.flush()
  
  len_out = len(zbuf)
  
  pnghdr = (ctypes.c_ubyte * 41)(
    0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
    0,0,w>>8,w,0,0,h>>8,h,8,ord("\0\0\04\02\06"[numchans]),0,0,0,0,0,0,0,
    len_out>>24,len_out>>16,len_out>>8,len_out,0x49,0x44,0x41,0x54)
  
  crc = zlib.crc32( ctypes.string_at( ctypes.byref(pnghdr,12), 17 ) )
  crc = ctypes.c_uint32( _htonl( crc ) )
  ctypes.memmove( ctypes.byref(pnghdr, 29), ctypes.pointer(crc), 4 )
  
  zbuf = ctypes.string_at( pnghdr, 41 ) + zbuf
  
  crc = zlib.crc32( zbuf[41-4:] )
  crc = _htonl(crc)
  crc = ctypes.c_int32(crc)
  
  zbuf += ctypes.string_at( ctypes.byref(crc), 4 )

  footer = "\x49\x45\x4e\x44\xae\x42\x60\x82"
  zbuf += footer

  return zbuf

def PNGTest():
  
  import os
  print os.getcwd()
  
  #rgba
  img = (ctypes.c_ubyte * (320 * 200 * 4))()
  for y in xrange(200):
    for x in xrange(320):
      img[y*320*4+x*4+0] = x & 255
      img[y*320*4+x*4+1] = (x*1) & 255
      img[y*320*4+x*4+2] = 255
      img[y*320*4+x*4+3] = y & 255
  
  imgstr = ctypes.string_at(img, 320 * 200 * 4)
  zbuf = compressPNG(imgstr, 320, 200, 4)
  print len(zbuf)
  with file("test.png", "wb") as f:
    f.write(zbuf)

PNGTest()

"""
// by alex evans, 2011.  released into the public domain.
// based on a first ever reading of the png spec, it occurs to me that a minimal png encoder should be quite simple.
// this is a first stab - may be buggy! the only external dependency is zlib and some basic typedefs (u32, u8)
//
// VERSION 0.02! now using zlib's crc rather than my own, and avoiding a memcpy and memory scribbler in the old one
// by passing the zero byte at the start of the scanline to zlib first, then the original scanline in place. WIN!
//
// more context at http://altdevblogaday.org/2011/04/06/a-smaller-jpg-encoder/.
//
// follow me on twitter @mmalex http://twitter.com/mmalex
//
u8* CompressPNG(void *img, int w, int h, int numchans, u32 &len_out) {
  int p=w*numchans;
  z_stream z={0}; 
  deflateInit(&z,-1);  
  u8 *zbuf=(u8*)malloc(53+(z.avail_out=deflateBound(&z,(1+p)*h))+1);  
  if (!zbuf) return 0;
  z.next_out=zbuf+41;  
  for (int y=0;y<h;++y) {
    Bytef zero=0; z.avail_in=1;z.next_in=&zero; deflate(&z,Z_NO_FLUSH);
    z.avail_in=p;z.next_in=((Bytef*)img)+y*p; deflate(&z,(y==h-1) ? Z_FINISH : Z_NO_FLUSH);
  }
  len_out=z.next_out-zbuf-41;
  u8 pnghdr[41]={0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
           0,0,w>>8,w,0,0,h>>8,h,8,"\0\0\04\02\06"[numchans],0,0,0,0,0,0,0,
           len_out>>24,len_out>>16,len_out>>8,len_out,0x49,0x44,0x41,0x54};
  *(u32*)(pnghdr+29)=htonl(crc32(0,pnghdr+12,17));  
  memcpy(zbuf,pnghdr,41); 
  memcpy(z.next_out+4,"\x49\x45\x4e\x44\xae\x42\x60\x82",8); // footer  
  *(u32*)z.next_out =htonl(crc32(0, zbuf+41-4, len_out+4));
  deflateEnd(&z); len_out+=53; return zbuf;
}
void PNGTest()
{
  u32 img[200][320];
  for (int y=0;y<200;++y) for (int x=0;x<320;++x) img[y][x]=x+(y<<8)+0xff000000;
  u32 blen; u8*zbuf = CompressPNG(img,16,16,4,blen);
  FILE *f=fopen("test.png","wb");
  fwrite(zbuf,1,blen,f);
  fclose(f);
  free(zbuf);
}
"""
