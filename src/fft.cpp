
#include "../include/fft/fft.h"

// from
// https://www.keithlantz.net/2011/11/ocean-simulation-part-two-using-the-fast-fourier-transform/
vector3::vector3() : x(0.0f), y(0.0f), z(0.0f) {}
vector3::vector3(float x, float y, float z) : x(x), y(y), z(z) {}

float vector3::operator*(const vector3 &v) {
  return this->x * v.x + this->y * v.y + this->z * v.z;
}

vector3 vector3::cross(const vector3 &v) {
  return vector3(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z,
                 this->x * v.y - this->y * v.z);
}

vector3 vector3::operator+(const vector3 &v) {
  return vector3(this->x + v.x, this->y + v.y, this->z + v.z);
}

vector3 vector3::operator-(const vector3 &v) {
  return vector3(this->x - v.x, this->y - v.y, this->z - v.z);
}

vector3 vector3::operator*(const float s) {
  return vector3(this->x * s, this->y * s, this->z * s);
}

vector3 &vector3::operator=(const vector3 &v) = default;

float vector3::length() {
  return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

vector3 vector3::unit() {
  float l = this->length();
  return vector3(this->x / l, this->y / l, this->z / l);
}

vector2::vector2() : x(0.0f), y(0.0f) {}
vector2::vector2(float x, float y) : x(x), y(y) {}

float vector2::operator*(const vector2 &v) {
  return this->x * v.x + this->y * v.y;
}

vector2 vector2::operator+(const vector2 &v) {
  return vector2(this->x + v.x, this->y + v.y);
}

vector2 vector2::operator-(const vector2 &v) {
  return vector2(this->x - v.x, this->y - v.y);
}

vector2 vector2::operator*(const float s) {
  return vector2(this->x * s, this->y * s);
}

vector2 &vector2::operator=(const vector2 &v) = default;

float vector2::length() { return sqrt(this->x * this->x + this->y * this->y); }

vector2 vector2::unit() {
  float l = this->length();
  return vector2(this->x / l, this->y / l);
}

cFFT::cFFT(unsigned int N) : N(N), reversed(nullptr), T(nullptr), pi2(2 * M_PI) {
  c[0] = c[1] = nullptr;

  log_2_N = log(N) / log(2);

  reversed = new unsigned int[N];  // prep bit reversals
  for (int i = 0; i < N; i++) reversed[i] = reverse(i);

  int pow2 = 1;
  T = new complex_t *[log_2_N];  // prep T
  for (int i = 0; i < log_2_N; i++) {
    T[i] = new complex_t[pow2];
    for (int j = 0; j < pow2; j++) T[i][j] = t(j, pow2 * 2);
    pow2 *= 2;
  }

  c[0] = new complex_t[N];
  c[1] = new complex_t[N];
  which = 0;
}

cFFT::~cFFT() {
  if (c[0]) delete[] c[0];
  if (c[1]) delete[] c[1];
  if (T) {
    for (int i = 0; i < log_2_N; i++)
      if (T[i]) delete[] T[i];
    delete[] T;
  }
  delete[] reversed;
}

unsigned int cFFT::reverse(unsigned int i) {
  unsigned int res = 0;
  for (int j = 0; j < log_2_N; j++) {
    res = (res << 1u) + (i & 1u);
    i >>= 1;
  }
  return res;
}

complex_t cFFT::t(unsigned int x, unsigned int N) {
  return complex_t(cos(pi2 * x / N), sin(pi2 * x / N));
}

void cFFT::fft(complex_t *input, complex_t *output, int stride, int offset) {
  for (int i = 0; i < N; i++)
    c[which][i] = input[reversed[i] * stride + offset];

  int loops = N >> 1u;
  int size = 1u << 1u;
  int size_over_2 = 1;
  int w_ = 0;
  for (int i = 1; i <= log_2_N; i++) {
    which ^= 1;
    for (int j = 0; j < loops; j++) {
      for (int k = 0; k < size_over_2; k++) {
        c[which][size * j + k] =
            c[which ^ 1u][size * j + k] +
                c[which ^ 1u][size * j + size_over_2 + k] * T[w_][k];
      }

      for (int k = size_over_2; k < size; k++) {
        c[which][size * j + k] =
            c[which ^ 1][size * j - size_over_2 + k] -
                c[which ^ 1][size * j + k] * T[w_][k - size_over_2];
      }
    }
    loops >>= 1;
    size <<= 1;
    size_over_2 <<= 1;
    w_++;
  }

  for (int i = 0; i < N; i++) output[i * stride + offset] = c[which][i];
}

cOcean::cOcean(const int N, const float A, const vector2 w, const float length,
               const bool geometry)
    : g(9.81),
      geometry(geometry),
      N(N),
      Nplus1(N + 1),
      A(A),
      w(w),
      length(length),
      vertices(nullptr),
      indices(nullptr),
      h_tilde(nullptr),
      h_tilde_slopex(nullptr),
      h_tilde_slopez(nullptr),
      h_tilde_dx(nullptr),
      h_tilde_dz(nullptr),
      fft(nullptr) {
  h_tilde = new complex_t[N * N];
  h_tilde_slopex = new complex_t[N * N];
  h_tilde_slopez = new complex_t[N * N];
  h_tilde_dx = new complex_t[N * N];
  h_tilde_dz = new complex_t[N * N];
  fft = new cFFT(N);
  vertices = new TrMapData<vertex_ocean>(Nplus1, Nplus1);
  vertices->m_rows = N;
  vertices->m_cols = N;
  indices = new unsigned int[Nplus1 * Nplus1 * 10];

  int index;

  complex_t htilde0, htilde0mk_conj;
  for (int m_prime = 0; m_prime < Nplus1; m_prime++) {
    for (int n_prime = 0; n_prime < Nplus1; n_prime++) {
      index = m_prime * Nplus1 + n_prime;

      htilde0 = hTilde_0(n_prime, m_prime);
      htilde0mk_conj = hTilde_0(-n_prime, -m_prime).conj();

      vertices->m_data[index].a = htilde0.a;
      vertices->m_data[index].b = htilde0.b;
      vertices->m_data[index]._a = htilde0mk_conj.a;
      vertices->m_data[index]._b = htilde0mk_conj.b;

      vertices->m_data[index].ox = vertices->m_data[index].x =
          (n_prime - N / 2.0f) * length / N;
      vertices->m_data[index].oy = vertices->m_data[index].y = 0.0f;
      vertices->m_data[index].oz = vertices->m_data[index].z =
          (m_prime - N / 2.0f) * length / N;

      vertices->m_data[index].nx = 0.0f;
      vertices->m_data[index].ny = 1.0f;
      vertices->m_data[index].nz = 0.0f;
    }
  }

  indices_count = 0;
  for (int m_prime = 0; m_prime < N; m_prime++) {
    for (int n_prime = 0; n_prime < N; n_prime++) {
      index = m_prime * Nplus1 + n_prime;

      if (geometry) {
        indices[indices_count++] = index;  // lines
        indices[indices_count++] = index + 1;
        indices[indices_count++] = index;
        indices[indices_count++] = index + Nplus1;
        indices[indices_count++] = index;
        indices[indices_count++] = index + Nplus1 + 1;
        if (n_prime == N - 1) {
          indices[indices_count++] = index + 1;
          indices[indices_count++] = index + Nplus1 + 1;
        }
        if (m_prime == N - 1) {
          indices[indices_count++] = index + Nplus1;
          indices[indices_count++] = index + Nplus1 + 1;
        }
      } else {
        indices[indices_count++] = index;  // two triangles
        indices[indices_count++] = index + Nplus1;
        indices[indices_count++] = index + Nplus1 + 1;
        indices[indices_count++] = index;
        indices[indices_count++] = index + Nplus1 + 1;
        indices[indices_count++] = index + 1;
      }
    }
  }
}

cOcean::~cOcean() {
  delete[] h_tilde;
  delete[] h_tilde_slopex;
  delete[] h_tilde_slopez;
  delete[] h_tilde_dx;
  delete[] h_tilde_dz;
  delete fft;
  delete vertices;
  delete[] indices;
}

void cOcean::release() {
}

float cOcean::dispersion(int n_prime, int m_prime) {
  float w_0 = 2.0f * M_PI / 200.0f;
  float kx = M_PI * (2 * n_prime - N) / length;
  float kz = M_PI * (2 * m_prime - N) / length;
  return floor(sqrt(g * sqrt(kx * kx + kz * kz)) / w_0) * w_0;
}

unsigned int complex_t::additions = 0;
unsigned int complex_t::multiplications = 0;

complex_t::complex_t() : a(0.0f), b(0.0f) {}
complex_t::complex_t(float a, float b) : a(a), b(b) {}
complex_t complex_t::conj() { return complex_t(this->a, -this->b); }

complex_t complex_t::operator*(const complex_t &c) const {
  complex_t::multiplications++;
  return complex_t(this->a * c.a - this->b * c.b,
                   this->a * c.b + this->b * c.a);
}

complex_t complex_t::operator+(const complex_t &c) const {
  complex_t::additions++;
  return complex_t(this->a + c.a, this->b + c.b);
}

complex_t complex_t::operator-(const complex_t &c) const {
  complex_t::additions++;
  return complex_t(this->a - c.a, this->b - c.b);
}

complex_t complex_t::operator-() const { return complex_t(-this->a, -this->b); }

complex_t complex_t::operator*(const float c) const {
  return complex_t(this->a * c, this->b * c);
}

complex_t &complex_t::operator=(const complex_t &c) = default;

void complex_t::reset() {
  complex_t::additions = 0;
  complex_t::multiplications = 0;
}

complex_t gaussianRandomVariable() {
  float x1, x2, w;
  do {
    x1 = 2.f * uniformRandomVariable() - 1.f;
    x2 = 2.f * uniformRandomVariable() - 1.f;
    w = x1 * x1 + x2 * x2;
  } while (w >= 1.f);
  w = sqrt((-2.f * log(w)) / w);
  return complex_t(x1 * w, x2 * w);
}

float uniformRandomVariable() { return (float) rand() / RAND_MAX; }

float cOcean::phillips(int n_prime, int m_prime) {
  vector2 k(M_PI * (2 * n_prime - N) / length,
            M_PI * (2 * m_prime - N) / length);
  float k_length = k.length();
  if (k_length < 0.000001) return 0.0;

  float k_length2 = k_length * k_length;
  float k_length4 = k_length2 * k_length2;

  float k_dot_w = k.unit() * w.unit();
  float k_dot_w2 = k_dot_w * k_dot_w * k_dot_w * k_dot_w * k_dot_w * k_dot_w;

  float w_length = w.length();
  float L = w_length * w_length / g;
  float L2 = L * L;

  float damping = 0.001;
  float l2 = L2 * damping * damping;

  return A * exp(-1.0f / (k_length2 * L2)) / k_length4 * k_dot_w2 *
      exp(-k_length2 * l2);
}

complex_t cOcean::hTilde_0(int n_prime, int m_prime) {
  complex_t r = gaussianRandomVariable();
  return r * sqrt(phillips(n_prime, m_prime) / 2.0f);
}

complex_t cOcean::hTilde(float t, int n_prime, int m_prime) {
  int index = m_prime * Nplus1 + n_prime;

  complex_t htilde0(vertices->m_data[index].a, vertices->m_data[index].b);
  complex_t htilde0mkconj(vertices->m_data[index]._a,
                          vertices->m_data[index]._b);

  float omegat = dispersion(n_prime, m_prime) * t;

  float cos_ = cos(omegat);
  float sin_ = sin(omegat);

  complex_t c0(cos_, sin_);
  complex_t c1(cos_, -sin_);

  complex_t res = htilde0 * c0 + htilde0mkconj * c1;

  return htilde0 * c0 + htilde0mkconj * c1;
}

complex_vector_normal cOcean::h_D_and_n(vector2 x, float t) {
  complex_t h(0.0f, 0.0f);
  vector2 D(0.0f, 0.0f);
  vector3 n(0.0f, 0.0f, 0.0f);

  complex_t c, res, htilde_c;
  vector2 k;
  float kx, kz, k_length, k_dot_x;

  for (int m_prime = 0; m_prime < N; m_prime++) {
    kz = 2.0f * M_PI * (m_prime - N / 2.0f) / length;
    for (int n_prime = 0; n_prime < N; n_prime++) {
      kx = 2.0f * M_PI * (n_prime - N / 2.0f) / length;
      k = vector2(kx, kz);

      k_length = k.length();
      k_dot_x = k * x;

      c = complex_t(cos(k_dot_x), sin(k_dot_x));
      htilde_c = hTilde(t, n_prime, m_prime) * c;

      h = h + htilde_c;

      n = n + vector3(-kx * htilde_c.b, 0.0f, -kz * htilde_c.b);

      if (k_length < 0.000001) continue;
      D = D + vector2(kx / k_length * htilde_c.b, kz / k_length * htilde_c.b);
    }
  }

  n = (vector3(0.0f, 1.0f, 0.0f) - n).unit();

  complex_vector_normal cvn;
  cvn.h = h;
  cvn.D = D;
  cvn.n = n;
  return cvn;
}

void cOcean::evaluateWaves(float t) {
  float lambda = -1.0;
  int index;
  vector2 x;
  vector2 d;
  complex_vector_normal h_d_and_n;
  for (int m_prime = 0; m_prime < N; m_prime++) {
    for (int n_prime = 0; n_prime < N; n_prime++) {
      index = m_prime * Nplus1 + n_prime;

      x = vector2(vertices->m_data[index].x, vertices->m_data[index].z);

      h_d_and_n = h_D_and_n(x, t);

      vertices->m_data[index].y = h_d_and_n.h.a;

      vertices->m_data[index].x =
          vertices->m_data[index].ox + lambda * h_d_and_n.D.x;
      vertices->m_data[index].z =
          vertices->m_data[index].oz + lambda * h_d_and_n.D.y;

      vertices->m_data[index].nx = h_d_and_n.n.x;
      vertices->m_data[index].ny = h_d_and_n.n.y;
      vertices->m_data[index].nz = h_d_and_n.n.z;

      if (n_prime == 0 && m_prime == 0) {
        vertices->m_data[index + N + Nplus1 * N].y = h_d_and_n.h.a;

        vertices->m_data[index + N + Nplus1 * N].x =
            vertices->m_data[index + N + Nplus1 * N].ox +
                lambda * h_d_and_n.D.x;
        vertices->m_data[index + N + Nplus1 * N].z =
            vertices->m_data[index + N + Nplus1 * N].oz +
                lambda * h_d_and_n.D.y;

        vertices->m_data[index + N + Nplus1 * N].nx = h_d_and_n.n.x;
        vertices->m_data[index + N + Nplus1 * N].ny = h_d_and_n.n.y;
        vertices->m_data[index + N + Nplus1 * N].nz = h_d_and_n.n.z;
      }
      if (n_prime == 0) {
        vertices->m_data[index + N].y = h_d_and_n.h.a;

        vertices->m_data[index + N].x =
            vertices->m_data[index + N].ox + lambda * h_d_and_n.D.x;
        vertices->m_data[index + N].z =
            vertices->m_data[index + N].oz + lambda * h_d_and_n.D.y;

        vertices->m_data[index + N].nx = h_d_and_n.n.x;
        vertices->m_data[index + N].ny = h_d_and_n.n.y;
        vertices->m_data[index + N].nz = h_d_and_n.n.z;
      }
      if (m_prime == 0) {
        vertices->m_data[index + Nplus1 * N].y = h_d_and_n.h.a;

        vertices->m_data[index + Nplus1 * N].x =
            vertices->m_data[index + Nplus1 * N].ox + lambda * h_d_and_n.D.x;
        vertices->m_data[index + Nplus1 * N].z =
            vertices->m_data[index + Nplus1 * N].oz + lambda * h_d_and_n.D.y;

        vertices->m_data[index + Nplus1 * N].nx = h_d_and_n.n.x;
        vertices->m_data[index + Nplus1 * N].ny = h_d_and_n.n.y;
        vertices->m_data[index + Nplus1 * N].nz = h_d_and_n.n.z;
      }
    }
  }
}

void cOcean::evaluateWavesFFT(float t) {
  float kx, kz, len, lambda = -1.0f;
  int index, index1;

  for (int m_prime = 0; m_prime < N; m_prime++) {
    kz = M_PI * (2.0f * m_prime - N) / length;
    for (int n_prime = 0; n_prime < N; n_prime++) {
      kx = M_PI * (2 * n_prime - N) / length;
      len = sqrt(kx * kx + kz * kz);
      index = m_prime * N + n_prime;

      h_tilde[index] = hTilde(t, n_prime, m_prime);
      h_tilde_slopex[index] = h_tilde[index] * complex_t(0, kx);
      h_tilde_slopez[index] = h_tilde[index] * complex_t(0, kz);
      if (len < 0.000001f) {
        h_tilde_dx[index] = complex_t(0.0f, 0.0f);
        h_tilde_dz[index] = complex_t(0.0f, 0.0f);
      } else {
        h_tilde_dx[index] = h_tilde[index] * complex_t(0, -kx / len);
        h_tilde_dz[index] = h_tilde[index] * complex_t(0, -kz / len);
      }
    }
  }

  for (int m_prime = 0; m_prime < N; m_prime++) {
    fft->fft(h_tilde, h_tilde, 1, m_prime * N);
    fft->fft(h_tilde_slopex, h_tilde_slopex, 1, m_prime * N);
    fft->fft(h_tilde_slopez, h_tilde_slopez, 1, m_prime * N);
    fft->fft(h_tilde_dx, h_tilde_dx, 1, m_prime * N);
    fft->fft(h_tilde_dz, h_tilde_dz, 1, m_prime * N);
  }
  for (int n_prime = 0; n_prime < N; n_prime++) {
    fft->fft(h_tilde, h_tilde, N, n_prime);
    fft->fft(h_tilde_slopex, h_tilde_slopex, N, n_prime);
    fft->fft(h_tilde_slopez, h_tilde_slopez, N, n_prime);
    fft->fft(h_tilde_dx, h_tilde_dx, N, n_prime);
    fft->fft(h_tilde_dz, h_tilde_dz, N, n_prime);
  }

  int sign;
  float signs[] = {1.0f, -1.0f};
  vector3 n;
  for (int m_prime = 0; m_prime < N; m_prime++) {
    for (int n_prime = 0; n_prime < N; n_prime++) {
      index = m_prime * N + n_prime;        // index into h_tilde..
      index1 = m_prime * Nplus1 + n_prime;  // index into vertices

      sign = signs[(n_prime + m_prime) & 1];

      h_tilde[index] = h_tilde[index] * sign;

      // height
      vertices->m_data[index1].y = h_tilde[index].a;

      // displacement
      h_tilde_dx[index] = h_tilde_dx[index] * sign;
      h_tilde_dz[index] = h_tilde_dz[index] * sign;
      vertices->m_data[index1].x =
          vertices->m_data[index1].ox + h_tilde_dx[index].a * lambda;
      vertices->m_data[index1].z =
          vertices->m_data[index1].oz + h_tilde_dz[index].a * lambda;

      // normal
      h_tilde_slopex[index] = h_tilde_slopex[index] * sign;
      h_tilde_slopez[index] = h_tilde_slopez[index] * sign;
      n = vector3(0.0f - h_tilde_slopex[index].a, 1.0f,
                  0.0f - h_tilde_slopez[index].a)
          .unit();
      vertices->m_data[index1].nx = n.x;
      vertices->m_data[index1].ny = n.y;
      vertices->m_data[index1].nz = n.z;

      // for tiling
      if (n_prime == 0 && m_prime == 0) {
        vertices->m_data[index1 + N + Nplus1 * N].y = h_tilde[index].a;

        vertices->m_data[index1 + N + Nplus1 * N].x =
            vertices->m_data[index1 + N + Nplus1 * N].ox +
                h_tilde_dx[index].a * lambda;
        vertices->m_data[index1 + N + Nplus1 * N].z =
            vertices->m_data[index1 + N + Nplus1 * N].oz +
                h_tilde_dz[index].a * lambda;

        vertices->m_data[index1 + N + Nplus1 * N].nx = n.x;
        vertices->m_data[index1 + N + Nplus1 * N].ny = n.y;
        vertices->m_data[index1 + N + Nplus1 * N].nz = n.z;
      }
      if (n_prime == 0) {
        vertices->m_data[index1 + N].y = h_tilde[index].a;

        vertices->m_data[index1 + N].x =
            vertices->m_data[index1 + N].ox + h_tilde_dx[index].a * lambda;
        vertices->m_data[index1 + N].z =
            vertices->m_data[index1 + N].oz + h_tilde_dz[index].a * lambda;

        vertices->m_data[index1 + N].nx = n.x;
        vertices->m_data[index1 + N].ny = n.y;
        vertices->m_data[index1 + N].nz = n.z;
      }
      if (m_prime == 0) {
        vertices->m_data[index1 + Nplus1 * N].y = h_tilde[index].a;

        vertices->m_data[index1 + Nplus1 * N].x =
            vertices->m_data[index1 + Nplus1 * N].ox +
                h_tilde_dx[index].a * lambda;
        vertices->m_data[index1 + Nplus1 * N].z =
            vertices->m_data[index1 + Nplus1 * N].oz +
                h_tilde_dz[index].a * lambda;

        vertices->m_data[index1 + Nplus1 * N].nx = n.x;
        vertices->m_data[index1 + Nplus1 * N].ny = n.y;
        vertices->m_data[index1 + Nplus1 * N].nz = n.z;
      }
    }
  }
}

void cOcean::render(float t, bool use_fft) {
  static bool eval = false;
  if (!use_fft && !eval) {
    eval = true;
    evaluateWaves(t);
  } else if (use_fft) {
    evaluateWavesFFT(t);
  }

}