
#ifndef __VBX_LIB_COMMON_H__
#ifndef __VBX_LIB_H
#error This file should not be included directly, include vbx_lib.h instead
#endif
#define __VBX_LIB_COMMON_H__
//[almost] all these functions have prototypes in vbx_lib.h that declare them as static inline
void vbx_sync(){
	_vbx_sync();
}
void vbx_set_vl( int LENGTH )
{
	VBX_SET_VL( MOD_NONE, LENGTH, LENGTH );
}

void vbx_get_vl( int *LENGTH )
{
	VBX_GET( GET_VL, *LENGTH );
}

void vbx_get_mask_status( int *MASK_STATUS )
{
	VBX_GET_MASK( *MASK_STATUS );
}

void vbx_set_2D( int ROWS, int ID, int IA, int IB )
{
	VBX_SET_VL( MOD_2D,     ROWS, ID );
	VBX_SET_VL( MOD_ACC_2D, IA,   IB );
}

void vbx_set_3D( int MATS, int ID3D, int IA3D, int IB3D )
{
	VBX_SET_VL3D( MOD_2D,     MATS, ID3D );
	VBX_SET_VL3D( MOD_ACC_2D, IA3D, IB3D );
}

void vbx_get_2D( int *ROWS, int *ID, int *IA, int *IB )
{
	VBX_GET( GET_ROWS, *ROWS );
	VBX_GET( GET_ID,   *ID );
	VBX_GET( GET_IA,   *IA );
	VBX_GET( GET_IB,   *IB );
}

void vbx_get_3D( int *MATS, int *ID3D, int *IA3D, int *IB3D )
{
	VBX_GET( GET_MATS, *MATS );
	VBX_GET( GET_ID3D, *ID3D );
	VBX_GET( GET_IA3D, *IA3D );
	VBX_GET( GET_IB3D, *IB3D );
}

void vbx_dma_to_host( void *EXT, vbx_void_t *INT, int LENGTH )
{
	_vbx_dma_to_host(EXT,INT,LENGTH);
}

void vbx_dma_to_vector( vbx_void_t *INT, void* EXT, int LENGTH )
{
	_vbx_dma_to_vector(INT,EXT,LENGTH);
}

static inline void fake_dma_to_host_2D( void *dst, vbx_void_t *v_src, int32_t xlen, int32_t ylen, int32_t dst_stride, int32_t src_stride )
{
	int y;
	uint8_t *_dst=(uint8_t*)dst;
	uint8_t *_v_src=(uint8_t*)v_src;
	for( y=0; y<ylen; y++ ) {
		vbx_dma_to_host( _dst, _v_src, xlen );
		_dst   += dst_stride;
		_v_src += src_stride;
	}
}

static inline void fake_dma_to_vector_2D( vbx_void_t *v_dst, void *src, int32_t xlen, int32_t ylen, int32_t dst_stride, int32_t src_stride )
{
	int y;
	uint8_t *_v_dst=(uint8_t*)v_dst;
	uint8_t *_src=(uint8_t*)src;
	for( y=0; y<ylen; y++ ) {
		vbx_dma_to_vector( _v_dst, _src, xlen );
		_v_dst += dst_stride;
		_src   += src_stride;
	}
}

void vbx_dma_to_host_2D(void *dst, vbx_void_t *v_src, int32_t xlen, int32_t ylen,
                        int32_t dst_stride, int32_t src_stride )
{
#if VBX_SIMULATOR
		fake_dma_to_host_2D(dst, v_src, xlen, ylen, dst_stride, src_stride);
#else
	if(src_stride < xlen  || !VBX_DMA2D){
		fake_dma_to_host_2D(dst, v_src, xlen, ylen, dst_stride, src_stride);
	} else {
		_vbx_dma_to_host_2D(dst, v_src, xlen, ylen, dst_stride, src_stride);
	}
#endif
}

void vbx_dma_to_vector_2D(vbx_void_t *v_dst, void *src, int32_t xlen, int32_t ylen,
                          int32_t dst_stride, int32_t src_stride )
{
#if VBX_SIMULATOR
	fake_dma_to_vector_2D(v_dst, src, xlen, ylen, dst_stride, src_stride);
#else
	if(dst_stride < xlen || !VBX_DMA2D ){
        fake_dma_to_vector_2D(v_dst, src, xlen, ylen, dst_stride, src_stride);
	} else {
		_vbx_dma_to_vector_2D(v_dst, src, xlen, ylen, dst_stride, src_stride);
	}
#endif
}
#endif //__VBX_LIB_COMMON_H__
