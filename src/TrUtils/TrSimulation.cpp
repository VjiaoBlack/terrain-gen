/**
 * TrSimulation.cpp
 */

#include "TrSimulation.hpp"
#include "TrMath.hpp"

double daysAtMonth[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

dvec2 getSunPos(int month, int day, double hour) {
  double year = 2000;
  double min = 0;
  double sec = 0;
  // double lat = 56.5;
  double lat = 40;
  double lon = 6.5;

  double twopi = 2.0 * M_PI;
  double deg2rad = M_PI / 180.0;

  // Get day of the year, e.g. Feb 1 = 32, Mar 1 = 61 on leap years
  day = day + daysAtMonth[month];
  // leapdays = year %% 4 == 0 & (year %% 400 == 0 | year %% 100 != 0) & ;
  //             day >= 60 & !(month==2 & day==60)
  // day[leapdays] = day[leapdays] + 1;

  // Get Julian date - 2400000
  hour = hour + min / 60.0 + sec / 3600.0;  // hour plus fraction
  double delta = year - 1949;
  double leap = trunc(delta / 4);  // former leapyears
  double jd = 32916.5 + delta * 365.0 + leap + day + hour / 24.0;

  // The input to the Atronomer's almanach is the difference between
  // the Julian date and JD 2451545.0 (noon, 1 January 2000)
  // double tim = jd - 51545.; // which one is right?? what
  double tim = jd - 2451545.;

  // Ecliptic coordinates

  // Mean longitude
  double mnlong = 280.460 + .9856474 * tim;
  TR_ASSIGN_MOD(mnlong, 360);

  if (mnlong < 0) {
    mnlong += 360;
  }

  // Mean anomaly
  double mnanom = 357.528 + .9856003 * tim;
  TR_ASSIGN_MOD(mnanom, 360);

  if (mnanom < 0) {
    mnanom += 360;
  }
  mnanom = mnanom * deg2rad;

  // Ecliptic longitude and obliquity of ecliptic
  double eclong = mnlong + 1.915 * sin(mnanom) + 0.020 * sin(2 * mnanom);
  TR_ASSIGN_MOD(eclong, 360);

  if (eclong < 0) {
    eclong += 360;
  }
  double oblqec = 23.439 - 0.0000004 * tim;
  eclong = eclong * deg2rad;
  oblqec = oblqec * deg2rad;

  // Celestial coordinates
  // Right ascension and declination
  double num = cos(oblqec) * sin(eclong);
  double den = cos(eclong);
  double ra = atan(num / den);

  if (den < 0) {
    ra += M_PI;
  }
  if (den >= 0 && num < 0) {
    ra += twopi;
  }
  double dec = asin(sin(oblqec) * sin(eclong));

  // Local coordinates
  // Greenwich mean sidereal time
  double gmst = 6.697375 + .0657098242 * tim + hour;
  TR_ASSIGN_MOD(gmst, 24);
  if (gmst < 0) {
    gmst += 24.0;
  }

  // Local mean sidereal time
  double lmst = gmst + lon / 15.;
  TR_ASSIGN_MOD(lmst, 24);
  if (lmst < 0) {
    lmst += 24;
  }
  lmst = lmst * 15. * deg2rad;

  // Hour angle
  double ha = lmst - ra;
  if (ha < -M_PI) {
    ha += twopi;
  }
  if (ha > M_PI) {
    ha -= twopi;
  }

  // Latitude to radians
  lat = lat * deg2rad;

  // Azimuth and elevation
  double el = asin(sin(dec) * sin(lat) + cos(dec) * cos(lat) * cos(ha));
  double az = asin(-cos(dec) * sin(ha) / cos(el));

  // For logic and names, see Spencer, J.W. 1989. Solar Energy. 42(4):353
  bool cosAzPos = (0 <= sin(dec) - sin(el) * sin(lat));
  bool sinAzNeg = (sin(az) < 0);
  if (cosAzPos & sinAzNeg) {
    az += twopi;
  }
  // az[cosAzPos & sinAzNeg] = az[cosAzPos & sinAzNeg] + twopi;
  // az[!cosAzPos] = pi - az[!cosAzPos];
  if (!cosAzPos) {
    az = M_PI - az;
  }

  // if (0 < sin(dec) - sin(el) * sin(lat)) {
  //     if(sin(az) < 0) az = az + twopi;
  // } else {
  //     az = pi - az;
  // }

  /*
    m_light.z = sin(el);
    m_light.x = cos(el) * sin(az);
    m_light.y = cos(el) * cos(az);

    m_elevation = el;
  */

  return dvec2(az, el);
}

dvec2 getMoonPos(int month, int day, double hour) {
  double year = 2000;
  // int month = m_month;
  // double day = m_day;
  // double hour = m_hour;
  double min = 0;
  double sec = 0;
  // double lat = 56.5;
  double lat = 20;
  double lon = 6.5;

  double twopi = 2.0 * M_PI;
  double deg2rad = M_PI / 180.0;

  // Get day of the year, e.g. Feb 1 = 32, Mar 1 = 61 on leap years
  day = day + daysAtMonth[month];
  // leapdays = year %% 4 == 0 & (year %% 400 == 0 | year %% 100 != 0) & ;
  //             day >= 60 & !(month==2 & day==60)
  // day[leapdays] = day[leapdays] + 1;

  // Get Julian date - 2400000
  hour = hour + min / 60.0 + sec / 3600.0;  // hour plus fraction
  double delta = year - 1949;
  double leap = trunc(delta / 4);  // former leapyears

  double jd = 32916.5 + delta * 365.0 + leap + day + hour / 24.0;

  // The input to the Atronomer's almanach is the difference between
  // the Julian date and JD 2451545.0 (noon, 1 January 2000)
  // double tim = jd - 51545.; // which one is right?? what
  double tim = jd - 2451545.;

  // https://www.mathworks.com/matlabcentral/fileexchange/22992-lunar-azimuth-and-altitude-estimation-algorithm?focused=5160125&tab=function&s_tid=gn_loc_drop
  // Lunar Azimuth and Altitude Estimation Algorithm
  // version 1.4 (4.26 KB) by Darin Koblick
  // Predict the Lunar Azimuth and Altitude within +/- .2 deg of any lat and lon
  // for a given UTC

  // Declare Earth Equatorial Radius Measurements in km
  double EarthRadEq = 6378.1370;
  // Convert Universal Time to Ephemeris Time
  // Find the Day Number
  double d = tim;
  // Keplerian Elements of the Moon// This will also account for the Sun's
  // perturbation
  double N = 125.1228 - 0.0529538083 * d;  //    (Long asc. node deg)
  double i = 5.1454;  //                      (Inclination deg)
  double w = 318.0634 + 0.1643573223 * d;  //  (Arg. of perigee deg)
  double a =
      60.2666;  //                     (Mean distance (Earth's Equitorial Radii)
  double e = 0.054900;  //                     (Eccentricity)
  double M = 115.3654 + 13.0649929509 * d;  //    (Mean anomaly deg)
  TR_ASSIGN_MOD(M, 360);                    //    (Mean anomaly deg)

  double LMoon = N + w + M;   //(Moon's mean longitude deg)
  TR_ASSIGN_MOD(LMoon, 360);  //(Moon's mean longitude deg)
  double FMoon = LMoon - N;   //(Moon's argument of latitude)
  TR_ASSIGN_MOD(FMoon, 360);  //(Moon's argument of latitude)
  // Keplerian Elements of the Sun
  double wSun = 282.9404 + 4.70935 * 0.00001 * d;  // (longitude of perihelion)
  TR_ASSIGN_MOD(wSun, 360);                        // (longitude of perihelion)
  double MSun = 356.0470 + 0.9856002585 * d;       // (Sun mean anomaly)
  TR_ASSIGN_MOD(MSun, 360);                        // (Sun mean anomaly)
  double LSun = wSun + MSun;                       // (Sun's mean longitude)
  TR_ASSIGN_MOD(LSun, 360);                        // (Sun's mean longitude)

  double DMoon = LMoon - LSun;  // (Moon's mean elongation)

  // Calculate Lunar perturbations in Longitude
  double LunarPLon =
      -1.274 * sin((M - 2 * DMoon) * deg2rad) +
          .658 * sin(2 * DMoon * deg2rad) + -0.186 * sin(MSun * deg2rad) +
          -0.059 * sin((2 * M - 2 * DMoon) * deg2rad) +
          -0.057 * sin((M - 2 * DMoon + MSun) * deg2rad) +
          .053 * sin((M + 2 * DMoon) * deg2rad) +
          .046 * sin((2 * DMoon - MSun) * deg2rad) +
          .041 * sin((M - MSun) * deg2rad) + -0.035 * sin(DMoon * deg2rad) +
          -0.031 * sin((M + MSun) * deg2rad) +
          -0.015 * sin((2 * FMoon - 2 * DMoon) * deg2rad) +
          .011 * sin((M - 4 * DMoon) * deg2rad);
  // Calculate Lunar perturbations in Latitude
  double LunarPLat = -0.173 * sin((FMoon - 2 * DMoon) * deg2rad) +
      -0.055 * sin((M - FMoon - 2 * DMoon) * deg2rad) +
      -0.046 * sin((M + FMoon - 2 * DMoon) * deg2rad) +
      +0.033 * sin((FMoon + 2 * DMoon) * deg2rad) +
      +0.017 * sin((2 * M + FMoon) * deg2rad);
  // Calculate perturbations in Distance
  double LunarPDist =
      -0.58 * cos((M - 2 * DMoon) * deg2rad) + -0.46 * cos(2 * DMoon * deg2rad);
  //  Compute E, the eccentric anomaly
  // E0 is the eccentric anomaly approximation estimate // (this will initially
  // have a relativly high error)
  double E0 =
      M + (180.0 / M_PI) * e * sin(M * deg2rad) * (1 + e * cos(M * deg2rad));
  // Compute E1 and set it to E0 until the E1 == E0
  double E1 = E0 -
      (E0 - (180.0 / M_PI) * e * sin(E0 * deg2rad) - M) /
          (1 - e * cos(E0 * deg2rad));
  while (E1 - E0 > .000005) {
    E0 = E1;
    E1 = E0 -
        (E0 - (180.0 / M_PI) * e * sin(E0 * deg2rad) - M) /
            (1 - e * cos(E0 * deg2rad));
  }
  double E = E1;
  // Compute rectangular coordinates (x,y) in the plane of the lunar orbit
  double x = a * (cos(E * deg2rad) - e);
  double y = a * sqrt(1 - e * e) * sin(E * deg2rad);
  // convert this to distance and true anomaly
  double r = sqrt(x * x + y * y);
  double v = atan2(y * deg2rad, x * deg2rad) * (180.0 / M_PI);
  // Compute moon's position in ecliptic coordinates
  double xeclip =
      r * (cos(N * deg2rad) * cos((v + w) * deg2rad) -
          sin(N * deg2rad) * sin((v + w) * deg2rad) * cos(i * deg2rad));
  double yeclip =
      r * (sin(N * deg2rad) * cos((v + w) * deg2rad) +
          cos(N * deg2rad) * sin(((v + w) * deg2rad)) * cos(i * deg2rad));
  double zeclip = r * sin((v + w) * deg2rad) * sin(i * deg2rad);
  // Add the calculated lunar perturbation terms to increase model fidelity
  double eLon, eLat, eDist;
  TR_CART_TO_SPH(eLon, eLat, eDist, xeclip, yeclip, zeclip);
  eLon += LunarPLon * deg2rad;
  eLat += LunarPLat * deg2rad;
  eDist += LunarPDist;

  TR_SPH_TO_CART(eLon, eLat, eDist, xeclip, yeclip, zeclip);
  // clear eLon eLat eDist;
  // convert the latitude and longitude to right ascension RA and declination//
  // delta
  double T = (jd - 2451545.0) / 36525.0;
  // Generate a rotation matrix for ecliptic to equitorial//
  // RotM=rotm_coo('E',jd);// See rotm_coo.m for obl and rotational matrix
  // transformation
  double Obl =
      23.439291 - 0.0130042 * T - 0.00000016 * T * T + 0.000000504 * T * T * T;
  Obl = Obl * deg2rad;
  // double RotM = [1 0 0; 0 cos(Obl) sin(Obl); 0 -sin(Obl) cos(Obl)];
  dvec3 RotM1(1, 0, 0);
  dvec3 RotM2(0, cos(Obl), sin(Obl));
  dvec3 RotM3(0, -sin(Obl), cos(Obl));
  // Apply the rotational matrix to the ecliptic rectangular coordinates// Also,
  // convert units to km instead of earth equatorial radii

  dvec3 xyzeclip(xeclip, yeclip, zeclip);

  dvec3 sol(dot(RotM1, xyzeclip), dot(RotM2, xyzeclip), dot(RotM3, xyzeclip));

  sol = sol * EarthRadEq;

  // double xel, yel, zel;
  dvec3 xyzsl;
  // Find the equatorial rectangular coordinates of the location @ sea level
  // [xsl ysl zsl] = sph2cart(Lon*deg2rad,Lat*deg2rad,EarthRadEq);

  TR_SPH_TO_CART(lon, lat, EarthRadEq, xyzsl.x, xyzsl.y, xyzsl.z);

  // dvec3solmsl = sol - xyzsl;


  // Find the Angle Between sea level coordinate vector and the moon vector
  // double theta1 = 180.0 -
  //                 acos(xyzsl.dot(solmsl) /
  //                      (sqrt(xyzsl.dot(xyzsl)) * sqrt(solmsl.dot(solmsl)))) /
  //                     deg2rad;
  // Find the Angle Between the same coordinates but at the specified elevation
  // double theta2 = 180.0 - acosd(dot([xel yel zel],[sol(1)-xel sol(2)-yel
  // sol(3)-zel]) ...
  //     /(sqrt(xel.^2 + yel.^2 + zel.^2) ...
  //          *sqrt((sol(1)-xel).^2 + (sol(2)-yel).^2 + (sol(3)-zel).^2)));
  // Find the Difference Between the two angles (+|-) is important
  // double thetaDiff = theta2 - theta1;
  //  equatorial to horizon coordinate transformation
  double RA, temp;
  TR_CART_TO_SPH(RA, delta, temp, sol.x, sol.y, sol.z);
  // [RA,delta] = cart2sph(sol(1),sol(2),sol(3));
  delta = delta * (180.0 / M_PI);
  RA = RA * (180.0 / M_PI);
  // Following the RA DEC to Az Alt conversion sequence explained here://
  // http://www.stargazing.net/kepler/altaz.html
  // Find the J2000 value
  double J2000 = tim;
  double UTH = hour + min / 60.0 + sec / 3600.0;
  // Calculate local siderial time
  double LST = 100.46 + 0.985647 * J2000 + lon + 15 * UTH;
  TR_ASSIGN_MOD(LST, 360);
  // Replace RA with hour angle HA
  double HA = LST - RA;
  // Find the h and AZ at the current LST
  double h =
      asin(sin(delta * deg2rad) * sin(lat * deg2rad) +
          cos(delta * deg2rad) * cos(lat * deg2rad) * cos(HA * deg2rad)) *
          (180.0 / M_PI);
  double Az =
      acos((sin(delta * deg2rad) - sin(h * deg2rad) * sin(lat * deg2rad)) /
          (cos(h * deg2rad) * cos(lat * deg2rad))) *
          (180.0 / M_PI);
  // Add in the angle offset due to the specified site elevation
  // h = h + thetaDiff;

  if (sin(HA * deg2rad) >= 0) {
    Az = 360 - Az;
  }
  // Apply Paralax Correction if we are still on earth
  // double horParal = 8.794 / (r * 6379.14 / 149.59787e6);
  // double p = asin(cos(h * deg2rad) * sin((horParal / 3600) * deg2rad)) *
  //            (180.0 / M_PI);
  // h = h - p;

  /*
    m_moonlight.z = sin(h);
    m_moonlight.x = cos(h) * sin(Az);
    m_moonlight.y = cos(h) * cos(Az);

    m_elevation = el;
  */

  return dvec2(Az * deg2rad, h * deg2rad);
}
