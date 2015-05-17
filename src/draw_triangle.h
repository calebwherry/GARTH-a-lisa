namespace garth_kernels{

  // Jesus Christ
  const char draw_triangle[] = BOOST_COMPUTE_STRINGIZE_SOURCE(
    __kernel void draw_triangle(
      __global const float *a, 
      __global const float *b,
      __global float *c)
    {
      const uint i = get_global_id(0);
      c[i] = a[i] + b[i];
    }
  );

}
