/*
 Copyright (C) Intel Corp.  2006.  All Rights Reserved.
 Intel funded Tungsten Graphics (http://www.tungstengraphics.com) to
 develop this 3D driver.
 
 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:
 
 The above copyright notice and this permission notice (including the
 next paragraph) shall be included in all copies or substantial
 portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
 LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 **********************************************************************/
 /*
  * Authors:
  *   Keith Whitwell <keith@tungstengraphics.com>
  */
        

#ifndef BRW_STRUCTS_H
#define BRW_STRUCTS_H


/** Number of general purpose registers (VS, WM, etc) */
#define BRW_MAX_GRF 128

/** Number of message register file registers */
#define BRW_MAX_MRF 16


/* Command packets:
 */
struct header 
{
   GLuint length:16; 
   GLuint opcode:16; 
};


union header_union
{
   struct header bits;
   GLuint dword;
};

struct brw_3d_control
{   
   struct 
   {
      GLuint length:8;
      GLuint notify_enable:1;
      GLuint pad:3;
      GLuint wc_flush_enable:1; 
      GLuint depth_stall_enable:1; 
      GLuint operation:2; 
      GLuint opcode:16; 
   } header;
   
   struct
   {
      GLuint pad:2;
      GLuint dest_addr_type:1; 
      GLuint dest_addr:29; 
   } dest;
   
   GLuint dword2;   
   GLuint dword3;   
};

/* These seem to be passed around as function args, so it works out
 * better to keep them as #defines:
 */
#define BRW_FLUSH_READ_CACHE           0x1
#define BRW_FLUSH_STATE_CACHE          0x2
#define BRW_INHIBIT_FLUSH_RENDER_CACHE 0x4
#define BRW_FLUSH_SNAPSHOT_COUNTERS    0x8

struct brw_mi_flush
{
   GLuint flags:4;
   GLuint pad:12;
   GLuint opcode:16;
};

struct brw_vf_statistics
{
   GLuint statistics_enable:1;
   GLuint pad:15;
   GLuint opcode:16;
};



struct brw_binding_table_pointers
{
   struct header header;
   GLuint vs; 
   GLuint gs; 
   GLuint clp; 
   GLuint sf; 
   GLuint wm; 
};


struct brw_blend_constant_color
{
   struct header header;
   GLfloat blend_constant_color[4];  
};


struct brw_depthbuffer
{
   union header_union header;
   
   union {
      struct {
	 GLuint pitch:18; 
	 GLuint format:3; 
	 GLuint pad:2;
	 GLuint software_tiled_rendering_mode:2;
	 GLuint depth_offset_disable:1; 
	 GLuint tile_walk:1; 
	 GLuint tiled_surface:1; 
	 GLuint pad2:1;
	 GLuint surface_type:3; 
      } bits;
      GLuint dword;
   } dword1;
   
   GLuint dword2_base_addr; 
 
   union {
      struct {
	 GLuint pad:1;
	 GLuint mipmap_layout:1; 
	 GLuint lod:4; 
	 GLuint width:13; 
	 GLuint height:13; 
      } bits;
      GLuint dword;
   } dword3;

   union {
      struct {
	 GLuint pad:10;
	 GLuint min_array_element:11; 
	 GLuint depth:11; 
      } bits;
      GLuint dword;
   } dword4;
};

struct brw_depthbuffer_g4x
{
   union header_union header;
   
   union {
      struct {
	 GLuint pitch:18; 
	 GLuint format:3; 
	 GLuint pad:2;
	 GLuint software_tiled_rendering_mode:2;
	 GLuint depth_offset_disable:1; 
	 GLuint tile_walk:1; 
	 GLuint tiled_surface:1; 
	 GLuint pad2:1;
	 GLuint surface_type:3; 
      } bits;
      GLuint dword;
   } dword1;
   
   GLuint dword2_base_addr; 
 
   union {
      struct {
	 GLuint pad:1;
	 GLuint mipmap_layout:1; 
	 GLuint lod:4; 
	 GLuint width:13; 
	 GLuint height:13; 
      } bits;
      GLuint dword;
   } dword3;

   union {
      struct {
	 GLuint pad:10;
	 GLuint min_array_element:11; 
	 GLuint depth:11; 
      } bits;
      GLuint dword;
   } dword4;

   union {
      struct {
         GLuint xoffset:16;
         GLuint yoffset:16;
      } bits;
      GLuint dword;
   } dword5;   /* NEW in Integrated Graphics Device */
};

struct brw_drawrect
{
   struct header header;
   GLuint xmin:16; 
   GLuint ymin:16; 
   GLuint xmax:16; 
   GLuint ymax:16; 
   GLuint xorg:16;  
   GLuint yorg:16;  
};




struct brw_global_depth_offset_clamp
{
   struct header header;
   GLfloat depth_offset_clamp;  
};

struct brw_indexbuffer
{   
   union {
      struct
      {
	 GLuint length:8; 
	 GLuint index_format:2; 
	 GLuint cut_index_enable:1; 
	 GLuint pad:5; 
	 GLuint opcode:16; 
      } bits;
      GLuint dword;

   } header;

   GLuint buffer_start; 
   GLuint buffer_end; 
};

/* NEW in Integrated Graphics Device */
struct brw_aa_line_parameters
{
   struct header header;

   struct {
      GLuint aa_coverage_slope:8;
      GLuint pad0:8;
      GLuint aa_coverage_bias:8;
      GLuint pad1:8;
   } bits0;

   struct {
      GLuint aa_coverage_endcap_slope:8;
      GLuint pad0:8;
      GLuint aa_coverage_endcap_bias:8;
      GLuint pad1:8;
   } bits1;
};

struct brw_line_stipple
{   
   struct header header;
  
   struct
   {
      GLuint pattern:16; 
      GLuint pad:16;
   } bits0;
   
   struct
   {
      GLuint repeat_count:9; 
      GLuint pad:7;
      GLuint inverse_repeat_count:16; 
   } bits1;
};


struct brw_pipelined_state_pointers
{
   struct header header;
   
   struct {
      GLuint pad:5;
      GLuint offset:27; /* Offset from GENERAL_STATE_BASE */
   } vs;
   
   struct
   {
      GLuint enable:1;
      GLuint pad:4;
      GLuint offset:27; /* Offset from GENERAL_STATE_BASE */
   } gs;
   
   struct
   {
      GLuint enable:1;
      GLuint pad:4;
      GLuint offset:27; /* Offset from GENERAL_STATE_BASE */
   } clp;
   
   struct
   {
      GLuint pad:5;
      GLuint offset:27; /* Offset from GENERAL_STATE_BASE */
   } sf;

   struct
   {
      GLuint pad:5;
      GLuint offset:27; /* Offset from GENERAL_STATE_BASE */
   } wm;
   
   struct
   {
      GLuint pad:5;
      GLuint offset:27; /* Offset from GENERAL_STATE_BASE. KW: check me! */
   } cc;
};


struct brw_polygon_stipple_offset
{
   struct header header;

   struct {
      GLuint y_offset:5; 
      GLuint pad:3;
      GLuint x_offset:5; 
      GLuint pad0:19;
   } bits0;
};



struct brw_polygon_stipple
{
   struct header header;
   GLuint stipple[32];
};



struct brw_pipeline_select
{
   struct
   {
      GLuint pipeline_select:1;   
      GLuint pad:15;
      GLuint opcode:16;   
   } header;
};


struct brw_pipe_control
{
   struct
   {
      GLuint length:8;
      GLuint notify_enable:1;
      GLuint texture_cache_flush_enable:1;
      GLuint indirect_state_pointers_disable:1;
      GLuint instruction_state_cache_flush_enable:1;
      GLuint write_cache_flush_enable:1;
      GLuint depth_stall_enable:1;
      GLuint post_sync_operation:2;

      GLuint opcode:16;
   } header;

   struct
   {
      GLuint pad:2;
      GLuint dest_addr_type:1;
      GLuint dest_addr:29;
   } bits1;

   GLuint data0;
   GLuint data1;
};


struct brw_urb_fence
{
   struct
   {
      GLuint length:8;   
      GLuint vs_realloc:1;   
      GLuint gs_realloc:1;   
      GLuint clp_realloc:1;   
      GLuint sf_realloc:1;   
      GLuint vfe_realloc:1;   
      GLuint cs_realloc:1;   
      GLuint pad:2;
      GLuint opcode:16;   
   } header;

   struct
   {
      GLuint vs_fence:10;  
      GLuint gs_fence:10;  
      GLuint clp_fence:10;  
      GLuint pad:2;
   } bits0;

   struct
   {
      GLuint sf_fence:10;  
      GLuint vf_fence:10;  
      GLuint cs_fence:11;  
      GLuint pad:1;
   } bits1;
};

struct brw_cs_urb_state
{
   struct header header;

   struct
   {
      GLuint nr_urb_entries:3;   
      GLuint pad:1;
      GLuint urb_entry_size:5;   
      GLuint pad0:23;
   } bits0;
};

struct brw_constant_buffer
{
   struct
   {
      GLuint length:8;   
      GLuint valid:1;   
      GLuint pad:7;
      GLuint opcode:16;   
   } header;

   struct
   {
      GLuint buffer_length:6;   
      GLuint buffer_address:26;  
   } bits0;
};

struct brw_state_base_address
{
   struct header header;

   struct
   {
      GLuint modify_enable:1;
      GLuint pad:4;
      GLuint general_state_address:27;  
   } bits0;

   struct
   {
      GLuint modify_enable:1;
      GLuint pad:4;
      GLuint surface_state_address:27;  
   } bits1;

   struct
   {
      GLuint modify_enable:1;
      GLuint pad:4;
      GLuint indirect_object_state_address:27;  
   } bits2;

   struct
   {
      GLuint modify_enable:1;
      GLuint pad:11;
      GLuint general_state_upper_bound:20;  
   } bits3;

   struct
   {
      GLuint modify_enable:1;
      GLuint pad:11;
      GLuint indirect_object_state_upper_bound:20;  
   } bits4;
};

struct brw_state_prefetch
{
   struct header header;

   struct
   {
      GLuint prefetch_count:3;   
      GLuint pad:3;
      GLuint prefetch_pointer:26;  
   } bits0;
};

struct brw_system_instruction_pointer
{
   struct header header;

   struct
   {
      GLuint pad:4;
      GLuint system_instruction_pointer:28;  
   } bits0;
};




/* State structs for the various fixed function units:
 */


struct thread0
{
   GLuint pad0:1;
   GLuint grf_reg_count:3; 
   GLuint pad1:2;
   GLuint kernel_start_pointer:26; /* Offset from GENERAL_STATE_BASE */
};

struct thread1
{
   GLuint ext_halt_exception_enable:1; 
   GLuint sw_exception_enable:1; 
   GLuint mask_stack_exception_enable:1; 
   GLuint timeout_exception_enable:1; 
   GLuint illegal_op_exception_enable:1; 
   GLuint pad0:3;
   GLuint depth_coef_urb_read_offset:6;	/* WM only */
   GLuint pad1:2;
   GLuint floating_point_mode:1; 
   GLuint thread_priority:1; 
   GLuint binding_table_entry_count:8; 
   GLuint pad3:5;
   GLuint single_program_flow:1; 
};

struct thread2
{
   GLuint per_thread_scratch_space:4; 
   GLuint pad0:6;
   GLuint scratch_space_base_pointer:22; 
};

   
struct thread3
{
   GLuint dispatch_grf_start_reg:4; 
   GLuint urb_entry_read_offset:6; 
   GLuint pad0:1;
   GLuint urb_entry_read_length:6; 
   GLuint pad1:1;
   GLuint const_urb_entry_read_offset:6; 
   GLuint pad2:1;
   GLuint const_urb_entry_read_length:6; 
   GLuint pad3:1;
};



struct brw_clip_unit_state
{
   struct thread0 thread0;
   struct
   {
      GLuint pad0:7;
      GLuint sw_exception_enable:1;
      GLuint pad1:3;
      GLuint mask_stack_exception_enable:1;
      GLuint pad2:1;
      GLuint illegal_op_exception_enable:1;
      GLuint pad3:2;
      GLuint floating_point_mode:1;
      GLuint thread_priority:1;
      GLuint binding_table_entry_count:8;
      GLuint pad4:5;
      GLuint single_program_flow:1;
   } thread1;

   struct thread2 thread2;
   struct thread3 thread3;

   struct
   {
      GLuint pad0:9;
      GLuint gs_output_stats:1; /* not always */
      GLuint stats_enable:1; 
      GLuint nr_urb_entries:7; 
      GLuint pad1:1;
      GLuint urb_entry_allocation_size:5; 
      GLuint pad2:1;
      GLuint max_threads:5; 	/* may be less */
      GLuint pad3:2;
   } thread4;   
      
   struct
   {
      GLuint pad0:13;
      GLuint clip_mode:3; 
      GLuint userclip_enable_flags:8; 
      GLuint userclip_must_clip:1; 
      GLuint negative_w_clip_test:1;
      GLuint guard_band_enable:1; 
      GLuint viewport_z_clip_enable:1; 
      GLuint viewport_xy_clip_enable:1; 
      GLuint vertex_position_space:1; 
      GLuint api_mode:1; 
      GLuint pad2:1;
   } clip5;
   
   struct
   {
      GLuint pad0:5;
      GLuint clipper_viewport_state_ptr:27; 
   } clip6;

   
   GLfloat viewport_xmin;  
   GLfloat viewport_xmax;  
   GLfloat viewport_ymin;  
   GLfloat viewport_ymax;  
};

struct gen6_blend_state
{
   struct {
      GLuint dest_blend_factor:5;
      GLuint source_blend_factor:5;
      GLuint pad3:1;
      GLuint blend_func:3;
      GLuint pad2:1;
      GLuint ia_dest_blend_factor:5;
      GLuint ia_source_blend_factor:5;
      GLuint pad1:1;
      GLuint ia_blend_func:3;
      GLuint pad0:1;
      GLuint ia_blend_enable:1;
      GLuint blend_enable:1;
   } blend0;

   struct {
      GLuint post_blend_clamp_enable:1;
      GLuint pre_blend_clamp_enable:1;
      GLuint clamp_range:2;
      GLuint pad0:4;
      GLuint x_dither_offset:2;
      GLuint y_dither_offset:2;
      GLuint dither_enable:1;
      GLuint alpha_test_func:3;
      GLuint alpha_test_enable:1;
      GLuint pad1:1;
      GLuint logic_op_func:4;
      GLuint logic_op_enable:1;
      GLuint pad2:1;
      GLuint write_disable_b:1;
      GLuint write_disable_g:1;
      GLuint write_disable_r:1;
      GLuint write_disable_a:1;
      GLuint pad3:1;
      GLuint alpha_to_coverage_dither:1;
      GLuint alpha_to_one:1;
      GLuint alpha_to_coverage:1;
   } blend1;
};

struct gen6_color_calc_state
{
   struct {
      GLuint alpha_test_format:1;
      GLuint pad0:14;
      GLuint round_disable:1;
      GLuint bf_stencil_ref:8;
      GLuint stencil_ref:8;
   } cc0;

   union {
      GLfloat alpha_ref_f;
      struct {
	 GLuint ui:8;
	 GLuint pad0:24;
      } alpha_ref_fi;
   } cc1;

   GLfloat constant_r;
   GLfloat constant_g;
   GLfloat constant_b;
   GLfloat constant_a;
};

struct gen6_depth_stencil_state
{
   struct {
      GLuint pad0:3;
      GLuint bf_stencil_pass_depth_pass_op:3;
      GLuint bf_stencil_pass_depth_fail_op:3;
      GLuint bf_stencil_fail_op:3;
      GLuint bf_stencil_func:3;
      GLuint bf_stencil_enable:1;
      GLuint pad1:2;
      GLuint stencil_write_enable:1;
      GLuint stencil_pass_depth_pass_op:3;
      GLuint stencil_pass_depth_fail_op:3;
      GLuint stencil_fail_op:3;
      GLuint stencil_func:3;
      GLuint stencil_enable:1;
   } ds0;

   struct {
      GLuint bf_stencil_write_mask:8;
      GLuint bf_stencil_test_mask:8;
      GLuint stencil_write_mask:8;
      GLuint stencil_test_mask:8;
   } ds1;

   struct {
      GLuint pad0:26;
      GLuint depth_write_enable:1;
      GLuint depth_test_func:3;
      GLuint pad1:1;
      GLuint depth_test_enable:1;
   } ds2;
};

struct brw_cc_unit_state
{
   struct
   {
      GLuint pad0:3;
      GLuint bf_stencil_pass_depth_pass_op:3; 
      GLuint bf_stencil_pass_depth_fail_op:3; 
      GLuint bf_stencil_fail_op:3; 
      GLuint bf_stencil_func:3; 
      GLuint bf_stencil_enable:1; 
      GLuint pad1:2;
      GLuint stencil_write_enable:1; 
      GLuint stencil_pass_depth_pass_op:3; 
      GLuint stencil_pass_depth_fail_op:3; 
      GLuint stencil_fail_op:3; 
      GLuint stencil_func:3; 
      GLuint stencil_enable:1; 
   } cc0;

   
   struct
   {
      GLuint bf_stencil_ref:8; 
      GLuint stencil_write_mask:8; 
      GLuint stencil_test_mask:8; 
      GLuint stencil_ref:8; 
   } cc1;

   
   struct
   {
      GLuint logicop_enable:1; 
      GLuint pad0:10;
      GLuint depth_write_enable:1; 
      GLuint depth_test_function:3; 
      GLuint depth_test:1; 
      GLuint bf_stencil_write_mask:8; 
      GLuint bf_stencil_test_mask:8; 
   } cc2;

   
   struct
   {
      GLuint pad0:8;
      GLuint alpha_test_func:3; 
      GLuint alpha_test:1; 
      GLuint blend_enable:1; 
      GLuint ia_blend_enable:1; 
      GLuint pad1:1;
      GLuint alpha_test_format:1;
      GLuint pad2:16;
   } cc3;
   
   struct
   {
      GLuint pad0:5; 
      GLuint cc_viewport_state_offset:27; /* Offset from GENERAL_STATE_BASE */
   } cc4;
   
   struct
   {
      GLuint pad0:2;
      GLuint ia_dest_blend_factor:5; 
      GLuint ia_src_blend_factor:5; 
      GLuint ia_blend_function:3; 
      GLuint statistics_enable:1; 
      GLuint logicop_func:4; 
      GLuint pad1:11;
      GLuint dither_enable:1; 
   } cc5;

   struct
   {
      GLuint clamp_post_alpha_blend:1; 
      GLuint clamp_pre_alpha_blend:1; 
      GLuint clamp_range:2; 
      GLuint pad0:11;
      GLuint y_dither_offset:2; 
      GLuint x_dither_offset:2; 
      GLuint dest_blend_factor:5; 
      GLuint src_blend_factor:5; 
      GLuint blend_function:3; 
   } cc6;

   struct {
      union {
	 GLfloat f;  
	 GLubyte ub[4];
      } alpha_ref;
   } cc7;
};

struct brw_sf_unit_state
{
   struct thread0 thread0;
   struct thread1 thread1;
   struct thread2 thread2;
   struct thread3 thread3;

   struct
   {
      GLuint pad0:10;
      GLuint stats_enable:1; 
      GLuint nr_urb_entries:7; 
      GLuint pad1:1;
      GLuint urb_entry_allocation_size:5; 
      GLuint pad2:1;
      GLuint max_threads:6; 
      GLuint pad3:1;
   } thread4;   

   struct
   {
      GLuint front_winding:1; 
      GLuint viewport_transform:1; 
      GLuint pad0:3;
      GLuint sf_viewport_state_offset:27; /* Offset from GENERAL_STATE_BASE */
   } sf5;
   
   struct
   {
      GLuint pad0:9;
      GLuint dest_org_vbias:4; 
      GLuint dest_org_hbias:4; 
      GLuint scissor:1; 
      GLuint disable_2x2_trifilter:1; 
      GLuint disable_zero_pix_trifilter:1; 
      GLuint point_rast_rule:2; 
      GLuint line_endcap_aa_region_width:2; 
      GLuint line_width:4; 
      GLuint fast_scissor_disable:1; 
      GLuint cull_mode:2; 
      GLuint aa_enable:1; 
   } sf6;

   struct
   {
      GLuint point_size:11; 
      GLuint use_point_size_state:1; 
      GLuint subpixel_precision:1; 
      GLuint sprite_point:1; 
      GLuint pad0:10;
      GLuint aa_line_distance_mode:1;
      GLuint trifan_pv:2; 
      GLuint linestrip_pv:2; 
      GLuint tristrip_pv:2; 
      GLuint line_last_pixel_enable:1; 
   } sf7;

};

struct gen6_scissor_rect
{
   GLuint xmin:16;
   GLuint ymin:16;
   GLuint xmax:16;
   GLuint ymax:16;
};

struct brw_gs_unit_state
{
   struct thread0 thread0;
   struct thread1 thread1;
   struct thread2 thread2;
   struct thread3 thread3;

   struct
   {
      GLuint pad0:8;
      GLuint rendering_enable:1; /* for Ironlake */
      GLuint pad4:1;
      GLuint stats_enable:1; 
      GLuint nr_urb_entries:7; 
      GLuint pad1:1;
      GLuint urb_entry_allocation_size:5; 
      GLuint pad2:1;
      GLuint max_threads:5; 
      GLuint pad3:2;
   } thread4;   
      
   struct
   {
      GLuint sampler_count:3; 
      GLuint pad0:2;
      GLuint sampler_state_pointer:27; 
   } gs5;

   
   struct
   {
      GLuint max_vp_index:4; 
      GLuint pad0:12;
      GLuint svbi_post_inc_value:10;
      GLuint pad1:1;
      GLuint svbi_post_inc_enable:1;
      GLuint svbi_payload:1;
      GLuint discard_adjaceny:1;
      GLuint reorder_enable:1; 
      GLuint pad2:1;
   } gs6;
};


struct brw_vs_unit_state
{
   struct thread0 thread0;
   struct thread1 thread1;
   struct thread2 thread2;
   struct thread3 thread3;
   
   struct
   {
      GLuint pad0:10;
      GLuint stats_enable:1; 
      GLuint nr_urb_entries:7; 
      GLuint pad1:1;
      GLuint urb_entry_allocation_size:5; 
      GLuint pad2:1;
      GLuint max_threads:6; 
      GLuint pad3:1;
   } thread4;   

   struct
   {
      GLuint sampler_count:3; 
      GLuint pad0:2;
      GLuint sampler_state_pointer:27; 
   } vs5;

   struct
   {
      GLuint vs_enable:1; 
      GLuint vert_cache_disable:1; 
      GLuint pad0:30;
   } vs6;
};


struct brw_wm_unit_state
{
   struct thread0 thread0;
   struct thread1 thread1;
   struct thread2 thread2;
   struct thread3 thread3;
   
   struct {
      GLuint stats_enable:1; 
      GLuint depth_buffer_clear:1;
      GLuint sampler_count:3; 
      GLuint sampler_state_pointer:27; 
   } wm4;
   
   struct
   {
      GLuint enable_8_pix:1; 
      GLuint enable_16_pix:1; 
      GLuint enable_32_pix:1; 
      GLuint enable_con_32_pix:1;
      GLuint enable_con_64_pix:1;
      GLuint pad0:1;

      /* These next four bits are for Ironlake+ */
      GLuint fast_span_coverage_enable:1;
      GLuint depth_buffer_clear:1;
      GLuint depth_buffer_resolve_enable:1;
      GLuint hierarchical_depth_buffer_resolve_enable:1;

      GLuint legacy_global_depth_bias:1; 
      GLuint line_stipple:1; 
      GLuint depth_offset:1; 
      GLuint polygon_stipple:1; 
      GLuint line_aa_region_width:2; 
      GLuint line_endcap_aa_region_width:2; 
      GLuint early_depth_test:1; 
      GLuint thread_dispatch_enable:1; 
      GLuint program_uses_depth:1; 
      GLuint program_computes_depth:1; 
      GLuint program_uses_killpixel:1; 
      GLuint legacy_line_rast: 1; 
      GLuint transposed_urb_read_enable:1; 
      GLuint max_threads:7; 
   } wm5;
   
   GLfloat global_depth_offset_constant;  
   GLfloat global_depth_offset_scale;   
   
   /* for Ironlake only */
   struct {
      GLuint pad0:1;
      GLuint grf_reg_count_1:3; 
      GLuint pad1:2;
      GLuint kernel_start_pointer_1:26;
   } wm8;       

   struct {
      GLuint pad0:1;
      GLuint grf_reg_count_2:3; 
      GLuint pad1:2;
      GLuint kernel_start_pointer_2:26;
   } wm9;       

   struct {
      GLuint pad0:1;
      GLuint grf_reg_count_3:3; 
      GLuint pad1:2;
      GLuint kernel_start_pointer_3:26;
   } wm10;       
};

struct brw_sampler_default_color {
   GLfloat color[4];
};

struct gen5_sampler_default_color {
   uint8_t ub[4];
   float f[4];
   uint16_t hf[4];
   uint16_t us[4];
   int16_t s[4];
   uint8_t b[4];
};

struct brw_sampler_state
{
   
   struct
   {
      GLuint shadow_function:3; 
      GLuint lod_bias:11; 
      GLuint min_filter:3; 
      GLuint mag_filter:3; 
      GLuint mip_filter:2; 
      GLuint base_level:5; 
      GLuint min_mag_neq:1;
      GLuint lod_preclamp:1; 
      GLuint default_color_mode:1; 
      GLuint pad0:1;
      GLuint disable:1; 
   } ss0;

   struct
   {
      GLuint r_wrap_mode:3; 
      GLuint t_wrap_mode:3; 
      GLuint s_wrap_mode:3; 
      GLuint cube_control_mode:1;
      GLuint pad:2;
      GLuint max_lod:10; 
      GLuint min_lod:10; 
   } ss1;

   
   struct
   {
      GLuint pad:5;
      GLuint default_color_pointer:27; 
   } ss2;
   
   struct
   {
      GLuint non_normalized_coord:1;
      GLuint pad:12;
      GLuint address_round:6;
      GLuint max_aniso:3; 
      GLuint chroma_key_mode:1; 
      GLuint chroma_key_index:2; 
      GLuint chroma_key_enable:1; 
      GLuint monochrome_filter_width:3; 
      GLuint monochrome_filter_height:3; 
   } ss3;
};

struct gen7_sampler_state
{
   struct
   {
      GLuint aniso_algorithm:1;
      GLuint lod_bias:13;
      GLuint min_filter:3;
      GLuint mag_filter:3;
      GLuint mip_filter:2;
      GLuint base_level:5;
      GLuint pad1:1;
      GLuint lod_preclamp:1;
      GLuint default_color_mode:1;
      GLuint pad0:1;
      GLuint disable:1;
   } ss0;

   struct
   {
      GLuint cube_control_mode:1;
      GLuint shadow_function:3;
      GLuint pad:4;
      GLuint max_lod:12;
      GLuint min_lod:12;
   } ss1;

   struct
   {
      GLuint pad:5;
      GLuint default_color_pointer:27;
   } ss2;

   struct
   {
      GLuint r_wrap_mode:3;
      GLuint t_wrap_mode:3;
      GLuint s_wrap_mode:3;
      GLuint pad:1;
      GLuint non_normalized_coord:1;
      GLuint trilinear_quality:2;
      GLuint address_round:6;
      GLuint max_aniso:3;
      GLuint chroma_key_mode:1;
      GLuint chroma_key_index:2;
      GLuint chroma_key_enable:1;
      GLuint pad0:6;
   } ss3;
};

struct brw_clipper_viewport
{
   GLfloat xmin;  
   GLfloat xmax;  
   GLfloat ymin;  
   GLfloat ymax;  
};

struct brw_cc_viewport
{
   GLfloat min_depth;  
   GLfloat max_depth;  
};

struct brw_sf_viewport
{
   struct {
      GLfloat m00;  
      GLfloat m11;  
      GLfloat m22;  
      GLfloat m30;  
      GLfloat m31;  
      GLfloat m32;  
   } viewport;

   /* scissor coordinates are inclusive */
   struct {
      GLshort xmin;
      GLshort ymin;
      GLshort xmax;
      GLshort ymax;
   } scissor;
};

struct gen6_sf_viewport {
   GLfloat m00;
   GLfloat m11;
   GLfloat m22;
   GLfloat m30;
   GLfloat m31;
   GLfloat m32;
};

struct gen7_sf_clip_viewport {
   struct {
      GLfloat m00;
      GLfloat m11;
      GLfloat m22;
      GLfloat m30;
      GLfloat m31;
      GLfloat m32;
   } viewport;

   GLuint pad0[2];

   struct {
      GLfloat xmin;
      GLfloat xmax;
      GLfloat ymin;
      GLfloat ymax;
   } guardband;

   GLfloat pad1[4];
};

/* volume 5c Shared Functions - 1.13.4.1.2 */
struct gen7_surface_state
{
   struct {
      GLuint cube_pos_z:1;
      GLuint cube_neg_z:1;
      GLuint cube_pos_y:1;
      GLuint cube_neg_y:1;
      GLuint cube_pos_x:1;
      GLuint cube_neg_x:1;
      GLuint pad2:2;
      GLuint render_cache_read_write:1;
      GLuint pad1:1;
      GLuint surface_array_spacing:1;
      GLuint vert_line_stride_ofs:1;
      GLuint vert_line_stride:1;
      GLuint tile_walk:1;
      GLuint tiled_surface:1;
      GLuint horizontal_alignment:1;
      GLuint vertical_alignment:2;
      GLuint surface_format:9;     /**< BRW_SURFACEFORMAT_x */
      GLuint pad0:1;
      GLuint is_array:1;
      GLuint surface_type:3;       /**< BRW_SURFACE_1D/2D/3D/CUBE */
   } ss0;

   struct {
      GLuint base_addr;
   } ss1;

   struct {
      GLuint width:14;
      GLuint pad1:2;
      GLuint height:14;
      GLuint pad0:2;
   } ss2;

   struct {
      GLuint pitch:18;
      GLuint pad:3;
      GLuint depth:11;
   } ss3;

   struct {
      GLuint multisample_position_palette_index:3;
      GLuint num_multisamples:3;
      GLuint multisampled_surface_storage_format:1;
      GLuint render_target_view_extent:11;
      GLuint min_array_elt:11;
      GLuint rotation:2;
      GLuint pad0:1;
   } ss4;

   struct {
      GLuint mip_count:4;
      GLuint min_lod:4;
      GLuint pad1:12;
      GLuint y_offset:4;
      GLuint pad0:1;
      GLuint x_offset:7;
   } ss5;

   struct {
      GLuint pad; /* Multisample Control Surface stuff */
   } ss6;

   struct {
      GLuint resource_min_lod:12;
      GLuint pad0:16;
      GLuint alpha_clear_color:1;
      GLuint blue_clear_color:1;
      GLuint green_clear_color:1;
      GLuint red_clear_color:1;
   } ss7;
};


struct brw_vertex_element_state
{
   struct
   {
      GLuint src_offset:11; 
      GLuint pad:5;
      GLuint src_format:9; 
      GLuint pad0:1;
      GLuint valid:1; 
      GLuint vertex_buffer_index:5; 
   } ve0;
   
   struct
   {
      GLuint dst_offset:8; 
      GLuint pad:8;
      GLuint vfcomponent3:4; 
      GLuint vfcomponent2:4; 
      GLuint vfcomponent1:4; 
      GLuint vfcomponent0:4; 
   } ve1;
};

#define BRW_VEP_MAX 18

struct brw_vertex_element_packet {
   struct header header;
   struct brw_vertex_element_state ve[BRW_VEP_MAX]; /* note: less than _TNL_ATTRIB_MAX */
};


struct brw_urb_immediate {
   GLuint opcode:4;
   GLuint offset:6;
   GLuint swizzle_control:2; 
   GLuint pad:1;
   GLuint allocate:1;
   GLuint used:1;
   GLuint complete:1;
   GLuint response_length:4;
   GLuint msg_length:4;
   GLuint msg_target:4;
   GLuint pad1:3;
   GLuint end_of_thread:1;
};

/* Instruction format for the execution units:
 */
 
struct brw_instruction
{
   struct 
   {
      GLuint opcode:7;
      GLuint pad:1;
      GLuint access_mode:1;
      GLuint mask_control:1;
      GLuint dependency_control:2;
      GLuint compression_control:2; /* gen6: quater control */
      GLuint thread_control:2;
      GLuint predicate_control:4;
      GLuint predicate_inverse:1;
      GLuint execution_size:3;
      GLuint destreg__conditionalmod:4; /* destreg - send, conditionalmod - others */
      GLuint acc_wr_control:1;
      GLuint cmpt_control:1;
      GLuint debug_control:1;
      GLuint saturate:1;
   } header;

   union {
      struct
      {
	 GLuint dest_reg_file:2;
	 GLuint dest_reg_type:3;
	 GLuint src0_reg_file:2;
	 GLuint src0_reg_type:3;
	 GLuint src1_reg_file:2;
	 GLuint src1_reg_type:3;
	 GLuint pad:1;
	 GLuint dest_subreg_nr:5;
	 GLuint dest_reg_nr:8;
	 GLuint dest_horiz_stride:2;
	 GLuint dest_address_mode:1;
      } da1;

      struct
      {
	 GLuint dest_reg_file:2;
	 GLuint dest_reg_type:3;
	 GLuint src0_reg_file:2;
	 GLuint src0_reg_type:3;
	 GLuint src1_reg_file:2;        /* 0x00000c00 */
	 GLuint src1_reg_type:3;        /* 0x00007000 */
	 GLuint pad:1;
	 GLint dest_indirect_offset:10;	/* offset against the deref'd address reg */
	 GLuint dest_subreg_nr:3; /* subnr for the address reg a0.x */
	 GLuint dest_horiz_stride:2;
	 GLuint dest_address_mode:1;
      } ia1;

      struct
      {
	 GLuint dest_reg_file:2;
	 GLuint dest_reg_type:3;
	 GLuint src0_reg_file:2;
	 GLuint src0_reg_type:3;
	 GLuint src1_reg_file:2;
	 GLuint src1_reg_type:3;
	 GLuint pad:1;
	 GLuint dest_writemask:4;
	 GLuint dest_subreg_nr:1;
	 GLuint dest_reg_nr:8;
	 GLuint dest_horiz_stride:2;
	 GLuint dest_address_mode:1;
      } da16;

      struct
      {
	 GLuint dest_reg_file:2;
	 GLuint dest_reg_type:3;
	 GLuint src0_reg_file:2;
	 GLuint src0_reg_type:3;
	 GLuint pad0:6;
	 GLuint dest_writemask:4;
	 GLint dest_indirect_offset:6;
	 GLuint dest_subreg_nr:3;
	 GLuint dest_horiz_stride:2;
	 GLuint dest_address_mode:1;
      } ia16;

      struct {
	 GLuint dest_reg_file:2;
	 GLuint dest_reg_type:3;
	 GLuint src0_reg_file:2;
	 GLuint src0_reg_type:3;
	 GLuint src1_reg_file:2;
	 GLuint src1_reg_type:3;
	 GLuint pad:1;

	 GLint jump_count:16;
      } branch_gen6;
   } bits1;


   union {
      struct
      {
	 GLuint src0_subreg_nr:5;
	 GLuint src0_reg_nr:8;
	 GLuint src0_abs:1;
	 GLuint src0_negate:1;
	 GLuint src0_address_mode:1;
	 GLuint src0_horiz_stride:2;
	 GLuint src0_width:3;
	 GLuint src0_vert_stride:4;
	 GLuint flag_reg_nr:1;
	 GLuint pad:6;
      } da1;

      struct
      {
	 GLint src0_indirect_offset:10;
	 GLuint src0_subreg_nr:3;
	 GLuint src0_abs:1;
	 GLuint src0_negate:1;
	 GLuint src0_address_mode:1;
	 GLuint src0_horiz_stride:2;
	 GLuint src0_width:3;
	 GLuint src0_vert_stride:4;
	 GLuint flag_reg_nr:1;
	 GLuint pad:6;	
      } ia1;

      struct
      {
	 GLuint src0_swz_x:2;
	 GLuint src0_swz_y:2;
	 GLuint src0_subreg_nr:1;
	 GLuint src0_reg_nr:8;
	 GLuint src0_abs:1;
	 GLuint src0_negate:1;
	 GLuint src0_address_mode:1;
	 GLuint src0_swz_z:2;
	 GLuint src0_swz_w:2;
	 GLuint pad0:1;
	 GLuint src0_vert_stride:4;
	 GLuint flag_reg_nr:1;
	 GLuint pad1:6;
      } da16;

      struct
      {
	 GLuint src0_swz_x:2;
	 GLuint src0_swz_y:2;
	 GLint src0_indirect_offset:6;
	 GLuint src0_subreg_nr:3;
	 GLuint src0_abs:1;
	 GLuint src0_negate:1;
	 GLuint src0_address_mode:1;
	 GLuint src0_swz_z:2;
	 GLuint src0_swz_w:2;
	 GLuint pad0:1;
	 GLuint src0_vert_stride:4;
	 GLuint flag_reg_nr:1;
	 GLuint pad1:6;
      } ia16;

       struct 
       {
           GLuint pad:26;
           GLuint end_of_thread:1;
           GLuint pad1:1;
           GLuint sfid:4;
       } send_gen5;  /* for Ironlake only */

   } bits2;

   union
   {
      struct
      {
	 GLuint src1_subreg_nr:5;
	 GLuint src1_reg_nr:8;
	 GLuint src1_abs:1;
	 GLuint src1_negate:1;
	 GLuint src1_address_mode:1;
	 GLuint src1_horiz_stride:2;
	 GLuint src1_width:3;
	 GLuint src1_vert_stride:4;
	 GLuint pad0:7;
      } da1;

      struct
      {
	 GLuint src1_swz_x:2;
	 GLuint src1_swz_y:2;
	 GLuint src1_subreg_nr:1;
	 GLuint src1_reg_nr:8;
	 GLuint src1_abs:1;
	 GLuint src1_negate:1;
	 GLuint src1_address_mode:1;
	 GLuint src1_swz_z:2;
	 GLuint src1_swz_w:2;
	 GLuint pad1:1;
	 GLuint src1_vert_stride:4;
	 GLuint pad2:7;
      } da16;

      struct
      {
	 GLint  src1_indirect_offset:10;
	 GLuint src1_subreg_nr:3;
	 GLuint src1_abs:1;
	 GLuint src1_negate:1;
	 GLuint src1_address_mode:1;
	 GLuint src1_horiz_stride:2;
	 GLuint src1_width:3;
	 GLuint src1_vert_stride:4;
	 GLuint flag_reg_nr:1;
	 GLuint pad1:6;	
      } ia1;

      struct
      {
	 GLuint src1_swz_x:2;
	 GLuint src1_swz_y:2;
	 GLint  src1_indirect_offset:6;
	 GLuint src1_subreg_nr:3;
	 GLuint src1_abs:1;
	 GLuint src1_negate:1;
	 GLuint pad0:1;
	 GLuint src1_swz_z:2;
	 GLuint src1_swz_w:2;
	 GLuint pad1:1;
	 GLuint src1_vert_stride:4;
	 GLuint flag_reg_nr:1;
	 GLuint pad2:6;
      } ia16;


      struct
      {
	 GLint  jump_count:16;	/* note: signed */
	 GLuint  pop_count:4;
	 GLuint  pad0:12;
      } if_else;

      /* This is also used for gen7 IF/ELSE instructions */
      struct
      {
	 /* Signed jump distance to the ip to jump to if all channels
	  * are disabled after the break or continue.  It should point
	  * to the end of the innermost control flow block, as that's
	  * where some channel could get re-enabled.
	  */
	 int jip:16;

	 /* Signed jump distance to the location to resume execution
	  * of this channel if it's enabled for the break or continue.
	  */
	 int uip:16;
      } break_cont;

      struct {
	 GLuint function:4;
	 GLuint int_type:1;
	 GLuint precision:1;
	 GLuint saturate:1;
	 GLuint data_type:1;
	 GLuint pad0:8;
	 GLuint response_length:4;
	 GLuint msg_length:4;
	 GLuint msg_target:4;
	 GLuint pad1:3;
	 GLuint end_of_thread:1;
      } math;

      struct {
	 GLuint function:4;
	 GLuint int_type:1;
	 GLuint precision:1;
	 GLuint saturate:1;
	 GLuint data_type:1;
	 GLuint snapshot:1;
	 GLuint pad0:10;
	 GLuint header_present:1;
	 GLuint response_length:5;
	 GLuint msg_length:4;
	 GLuint pad1:2;
	 GLuint end_of_thread:1;
      } math_gen5;

      struct {
	 GLuint binding_table_index:8;
	 GLuint sampler:4;
	 GLuint return_format:2; 
	 GLuint msg_type:2;   
	 GLuint response_length:4;
	 GLuint msg_length:4;
	 GLuint msg_target:4;
	 GLuint pad1:3;
	 GLuint end_of_thread:1;
      } sampler;

      struct {
         GLuint binding_table_index:8;
         GLuint sampler:4;
         GLuint msg_type:4;
         GLuint response_length:4;
         GLuint msg_length:4;
         GLuint msg_target:4;
         GLuint pad1:3;
         GLuint end_of_thread:1;
      } sampler_g4x;

      struct {
	 GLuint binding_table_index:8;
	 GLuint sampler:4;
	 GLuint msg_type:4;
	 GLuint simd_mode:2;
	 GLuint pad0:1;
	 GLuint header_present:1;
	 GLuint response_length:5;
	 GLuint msg_length:4;
	 GLuint pad1:2;
	 GLuint end_of_thread:1;
      } sampler_gen5;

      struct {
	 GLuint binding_table_index:8;
	 GLuint sampler:4;
	 GLuint msg_type:5;
	 GLuint simd_mode:2;
	 GLuint header_present:1;
	 GLuint response_length:5;
	 GLuint msg_length:4;
	 GLuint pad1:2;
	 GLuint end_of_thread:1;
      } sampler_gen7;

      struct brw_urb_immediate urb;

      struct {
	 GLuint opcode:4;
	 GLuint offset:6;
	 GLuint swizzle_control:2; 
	 GLuint pad:1;
	 GLuint allocate:1;
	 GLuint used:1;
	 GLuint complete:1;
	 GLuint pad0:3;
	 GLuint header_present:1;
	 GLuint response_length:5;
	 GLuint msg_length:4;
	 GLuint pad1:2;
	 GLuint end_of_thread:1;
      } urb_gen5;

      struct {
	 GLuint opcode:3;
	 GLuint offset:11;
	 GLuint swizzle_control:1;
	 GLuint complete:1;
	 GLuint per_slot_offset:1;
	 GLuint pad0:2;
	 GLuint header_present:1;
	 GLuint response_length:5;
	 GLuint msg_length:4;
	 GLuint pad1:2;
	 GLuint end_of_thread:1;
      } urb_gen7;

      struct {
	 GLuint binding_table_index:8;
	 GLuint msg_control:4;  
	 GLuint msg_type:2;  
	 GLuint target_cache:2;    
	 GLuint response_length:4;
	 GLuint msg_length:4;
	 GLuint msg_target:4;
	 GLuint pad1:3;
	 GLuint end_of_thread:1;
      } dp_read;

      struct {
	 GLuint binding_table_index:8;
	 GLuint msg_control:3;
	 GLuint msg_type:3;
	 GLuint target_cache:2;
	 GLuint response_length:4;
	 GLuint msg_length:4;
	 GLuint msg_target:4;
	 GLuint pad1:3;
	 GLuint end_of_thread:1;
      } dp_read_g4x;

      struct {
	 GLuint binding_table_index:8;
	 GLuint msg_control:3;  
	 GLuint msg_type:3;  
	 GLuint target_cache:2;    
	 GLuint pad0:3;
	 GLuint header_present:1;
	 GLuint response_length:5;
	 GLuint msg_length:4;
	 GLuint pad1:2;
	 GLuint end_of_thread:1;
      } dp_read_gen5;

      struct {
	 GLuint binding_table_index:8;
	 GLuint msg_control:3;
	 GLuint pixel_scoreboard_clear:1;
	 GLuint msg_type:3;    
	 GLuint send_commit_msg:1;
	 GLuint response_length:4;
	 GLuint msg_length:4;
	 GLuint msg_target:4;
	 GLuint pad1:3;
	 GLuint end_of_thread:1;
      } dp_write;

      struct {
	 GLuint binding_table_index:8;
	 GLuint msg_control:3;
	 GLuint pixel_scoreboard_clear:1;
	 GLuint msg_type:3;    
	 GLuint send_commit_msg:1;
	 GLuint pad0:3;
	 GLuint header_present:1;
	 GLuint response_length:5;
	 GLuint msg_length:4;
	 GLuint pad1:2;
	 GLuint end_of_thread:1;
      } dp_write_gen5;

      /* Sandybridge DP for sample cache, constant cache, render cache */
      struct {
	 GLuint binding_table_index:8;
	 GLuint msg_control:5;
	 GLuint msg_type:3;
	 GLuint pad0:3;
	 GLuint header_present:1;
	 GLuint response_length:5;
	 GLuint msg_length:4;
	 GLuint pad1:2;
	 GLuint end_of_thread:1;
      } dp_sampler_const_cache;

      struct {
	 GLuint binding_table_index:8;
	 GLuint msg_control:3;
	 GLuint slot_group_select:1;
	 GLuint pixel_scoreboard_clear:1;
	 GLuint msg_type:4;
	 GLuint send_commit_msg:1;
	 GLuint pad0:1;
	 GLuint header_present:1;
	 GLuint response_length:5;
	 GLuint msg_length:4;
	 GLuint pad1:2;
	 GLuint end_of_thread:1;
      } gen6_dp;

      /* See volume vol5c.2 sections 2.11.2.1.5 and 2.11.21.2.2. */
      struct {
	 GLuint binding_table_index:8;
	 GLuint msg_control:3;
	 GLuint slot_group_select:1;
	 GLuint pixel_scoreboard_clear:1;
	 GLuint pad0:1;
	 GLuint msg_type:4;
	 GLuint pad1:1;
	 GLuint header_present:1;
	 GLuint response_length:5;
	 GLuint msg_length:4;
	 GLuint pad2:2;
	 GLuint end_of_thread:1;
      } gen7_dp;

      struct {
	 GLuint function_control:16;
	 GLuint response_length:4;
	 GLuint msg_length:4;
	 GLuint msg_target:4;
	 GLuint pad1:3;
	 GLuint end_of_thread:1;
      } generic;

      /* Of this struct, only end_of_thread is not present for gen6. */
      struct {
	 GLuint function_control:19;
	 GLuint header_present:1;
	 GLuint response_length:5;
	 GLuint msg_length:4;
	 GLuint pad1:2;
	 GLuint end_of_thread:1;
      } generic_gen5;

      GLint d;
      GLuint ud;
      float f;
   } bits3;
};


#endif
