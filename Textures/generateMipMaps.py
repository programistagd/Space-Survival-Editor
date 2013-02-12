from PIL import Image
chunkSize = 256
cols = 5
rows=4
img = Image.open("input.png")
currentW=cols*chunkSize
currentH=rows*chunkSize
for i in range(1,11):
	nImg = Image.new(img.mode, (currentW/2,currentH/2))
	for x in range(0,cols):
		for y in range(0,rows):
			off = chunkSize/(2**(i-1))
			cImg = img.crop((x*off,y*off,x*off+off,y*off+off))
			off = off/2
			cImg = cImg.resize((off,off),Image.ANTIALIAS)
			nImg.paste(cImg,(off*x,off*y))
	img=nImg
	img.save("mipmaps/mipmap"+str(i)+".png")
	currentW/=2
	currentH/=2
			
