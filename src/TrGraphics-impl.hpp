/**
 * TrGraphics-impl.hpp
 *
 * Includes implementation of <T> functions in TrGraphics.hpp
 */

#ifndef _TR_GRAPHICS_IMPL_HPP_
#define _TR_GRAPHICS_IMPL_HPP_

template<class T>
inline void TrPixels<T>::set(int r, int c, T p) {
    if (r < 0) r += m_rows;
    if (c < 0) c += m_cols;

    if (r >= m_rows) r -= m_rows;
    if (c >= m_cols) c -= m_cols;

    m_pixels[r*m_cols + c] = p;
}

template<class T>
inline T TrPixels<T>::get(int r, int c) {
    if (r < 0) r += m_rows;
    if (c < 0) c += m_cols;

    if (r >= m_rows) r -= m_rows;
    if (c >= m_cols) c -= m_cols;

    return m_pixels[r*m_cols + c];
}

template<class T>
inline T& TrPixels<T>::at(int r, int c) {
    if (r < 0) r += m_rows;
    if (c < 0) c += m_cols;

    if (r >= m_rows) r -= m_rows;
    if (c >= m_cols) c -= m_cols;

    return m_pixels[r*m_cols + c];
}

template<class T>
pair<T, T> TrPixels<T>::getMinMax() {
    T min = m_pixels[0];
    T max = m_pixels[0];
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            if (this->at(i,j) < min) {
                min = this->at(i,j);
            } else if (this->at(i,j) > max) {
                max = this->at(i,j);
            }
        }
    }
    return make_pair(min, max);
}

template<class T>
void TrPixels<T>::set(T t) {
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            this->at(i,j) = t;
        }
    }
}

// Diamond Square algorithm.
template<class T>
void TrPixels<T>::diamondSquare(int s, double level) {
    if (s < 1) {
        return;
    }

    std::random_device rd;
    std::default_random_engine eg(rd());
    std::uniform_real_distribution<double> dist(-level, level);

    int hs = s/2;
    for (int i = hs; i < K_MAP_SIZE + hs; i += s) {
        for (int j = hs; j < K_MAP_SIZE + hs; j += s) {
            double a = this->get(i-hs, j-hs);
            double b = this->get(i+hs, j-hs);
            double c = this->get(i-hs, j+hs);
            double d = this->get(i+hs, j+hs);
            this->at(i, j) = (a + b + c + d) / 4.0 + dist(eg);
        }
    }


    for (int i = 0; i < K_MAP_SIZE; i += s) {
        for (int j = 0; j < K_MAP_SIZE; j += s) {
            double a = this->get(i   , j);
            double b = this->get(i+ s, j);
            double c = this->get(i+hs, j-hs);
            double d = this->get(i+hs, j+hs);
            double f = this->get(i-hs, j+hs);
            double k = this->get(i   , j+ s);

            this->at(i+hs, j) = (a + b + c + d) / 4.0 + dist(eg);
            this->at(i, j+hs) = (f + d + a + k) / 4.0 + dist(eg);
        }
    }

    this->diamondSquare(hs, level / 2);
}

// good enough small blur
// even though it's technically incorrect since we immediately place back into the grid
template<class T>
void TrPixels<T>::boxBlur() {
    for (int i = 0; i < K_MAP_SIZE; i++) {
        for (int j = 0; j < K_MAP_SIZE; j++) {
            T sum = (this->get(i-1.0, j-1.0) +
                     this->get(i-1.0, j    ) +
                     this->get(i-1.0, j+1.0) +
                     this->get(i    , j-1.0) +
                     this->get(i    , j    ) +
                     this->get(i    , j+1.0) +
                     this->get(i+1.0, j-1.0) +
                     this->get(i+1.0, j    ) +
                     this->get(i+1.0, j+1.0));

            m_pixels[i * K_MAP_SIZE + j] = sum / 9.0;
        }
    }  
}

template<class T>
void TrPixels<T>::perlinNoise(unsigned int s, int level, double size, double magnitude) {
    // Create a PerlinNoise object with the reference permutation vector

    PerlinNoise perlin = PerlinNoise();

    if (!level) {
        return;
    }

    // Visit every pixel of the image and assign a color generated with Perlin noise
    for (unsigned int i = 0; i < s; ++i) {      // y
        for (unsigned int j = 0; j < s; ++j) {  // x
            double x = (double)j/((double)(s)); 
            double y = (double)i/((double)(s)); 

            // Typical Perlin noise
            double n = perlin.noise(1.0 + x * size, 1.0 + y * size, 0);
            m_pixels[i * K_MAP_SIZE + j] += n * magnitude * 1.2;
            
        }
    }

    this->perlinNoise(s, level-1, size * 2.0, magnitude * .4);
}

#endif