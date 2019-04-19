char *LoadJpegToBuffer(char *file, SIZE *pSize)
{
	int pad;
	char *buffer;
	long line_size, row_size;
	FILE *infile;
	JSAMPARRAY rows;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	infile = fopen(file, "rb");
	if(infile==NULL)
	{
		printf("Can't open file '%s'.", file);
		return NULL;
	}
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);
	cinfo.out_color_space = JCS_RGB;
	jpeg_start_decompress(&cinfo);
	pSize->cx = cinfo.output_width;
	pSize->cy = cinfo.output_height;
	line_size = cinfo.output_components * cinfo.output_width;
	pad = line_size % 4;
	pad = (pad ? 4-pad : 0);
	row_size = line_size + pad;
	buffer = (char *) malloc(row_size * cinfo.output_height);
	rows = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, line_size, 1);
	while (cinfo.output_scanline < cinfo.output_height)
	{
		long i, j, k;
		i = row_size * (cinfo.output_height - cinfo.output_scanline - 1);
		j = 0;
		jpeg_read_scanlines(&cinfo, rows, 1);
		for(j=0; j<line_size; j+=3)
		{
			buffer[i++] = rows[0][j+2];
			buffer[i++] = rows[0][j+1];
			buffer[i++] = rows[0][j];
		}
		for(k=0; k<pad; k++)
		{
			buffer[i++] = 0;
		}
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);
	return buffer;
}