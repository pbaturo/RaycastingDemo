# RaycastingDemo TODO

## Performance Observations
### Potential Reasons for Slowness
- **Pixel-by-Pixel Drawing**: The `put_pixel()` function calls `SDL_RenderDrawPoint()` for each individual pixel, which is inefficient. SDL's point drawing is slow for large numbers of pixels; batching into textures or using `SDL_RenderDrawLines()` would be faster.
- **Inefficient Ray Casting**: Each ray advances by 1 unit per iteration in a `while` loop, checking for collisions via `touch()` at every step. This can result in hundreds or thousands of iterations per ray, especially for long distances, leading to high CPU usage.
- **No Optimized Ray Marching**: The code doesn't use advanced techniques like Digital Differential Analyzer (DDA) for faster grid-based ray stepping, which could reduce iterations significantly.
- **Per-Frame Calculations**: With 1280 rays per frame at 60 FPS, and each ray potentially involving many distance calculations (`fixed_distance()` with `sqrt()` and `atan2()`), the computational load is high. No caching or precomputation is done.
- **SDL Renderer Overhead**: Clearing the screen and drawing without batching or using accelerated features (e.g., no texture rendering) adds overhead. The renderer is set to `SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC`, but individual point draws aren't optimized.
- **Map Size and Collision Checks**: The `touch()` function performs integer division and array access per step, which is fine for small maps but could scale poorly if the map grows.

## Rendering Performance Improvements
- [ ] Implement DDA (Digital Differential Analyzer) for faster raycasting instead of pixel-by-pixel stepping
- [ ] Replace individual `SDL_RenderDrawPoint` calls with batched rendering using `SDL_RenderDrawLines` or textures
- [ ] Render to an off-screen texture and copy to screen once per frame to reduce draw calls
- [ ] Precompute trigonometric values (sin/cos) for ray angles to avoid repeated calculations
- [ ] Cache wall distances or use lookup tables for common scenarios

## Code Structure Optimizations
- [ ] Profile performance with Instruments (Mac) or similar tools to identify bottlenecks
- [ ] Optimize `touch()` function for faster collision detection (e.g., bounds checking)
- [ ] Reduce floating-point operations in hot loops (e.g., use fixed-point math where possible)
- [ ] Implement level-of-detail rendering for distant walls
- [ ] Add multithreading for raycasting if single-threaded performance is insufficient

## SDL-Specific Improvements
- [ ] Ensure renderer is using hardware acceleration (`SDL_RENDERER_ACCELERATED`)
- [ ] Use `SDL_Texture` for wall rendering instead of drawing pixels directly
- [ ] Implement `SDL_RenderCopy` for efficient texture blitting
- [ ] Set `SDL_SetRenderTarget` to render off-screen to a texture
- [ ] Replace `SDL_RenderDrawPoint` with `SDL_RenderDrawLines` for vertical wall slices
- [ ] Use `SDL_RenderGeometry` for batching complex shapes if needed
- [ ] Optimize renderer with `SDL_RENDERER_ACCELERATED` and `SDL_RENDERER_PRESENTVSYNC`
- [ ] Convert pixel data to `SDL_Surface` then to texture for faster manipulation
- [ ] Minimize `SDL_SetRenderDrawColor` calls by grouping draws by color

## General Enhancements
- [ ] Add frame rate counter to monitor performance improvements
- [ ] Test on different hardware to validate optimizations
- [ ] Consider using SDL_gpu or similar libraries for advanced rendering features

## New Features
- [ ] Add wall texturing to replace solid colors with images
- [ ] Implement ceiling rendering for a complete 3D environment
- [ ] Implement floor rendering with perspective correction
- [ ] Add sprite rendering for objects (e.g., enemies, items)
- [ ] Introduce basic lighting effects (e.g., distance-based shading)
- [ ] Support for multiple wall types and textures
- [ ] Add fog or depth effects for enhanced realism