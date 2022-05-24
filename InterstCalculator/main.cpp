#include <cmath>
#include <vector>
#include <iostream>
#include <cstdio>
#include <iomanip>


class BondTypes {

public:

    // --------------------------part A------------------------------------
    double ZeroCouponBond(int years, int facevalue, float YTM, int periods) {
        return facevalue / std::pow((1 + (YTM / periods)), years * periods);
    }

    //-----------------------------part C---------------------------------
    float CouponBondPrice(int years, int facevalue, float YTM, int periods, float coupon) {
        float couponvalue = (coupon * facevalue);
        float power = 1 / (std::pow((1 + (YTM / periods)), (years * periods)));
        float discount = (1 - power) / (YTM / periods);
        float discountedFV = (facevalue / std::pow((1 + (YTM / periods)), years * periods));
        float couponBond = couponvalue * discount + discountedFV;
        return couponBond;
    }

};

class Durations : public BondTypes {

public:

    //---------------------------part B-----------------------------------
    double FiniteDifferenceDuration1(int years, int facevalue, float YTM, int periods, float delta) {
        float priceDelta_plus0 = ZeroCouponBond(years, facevalue, YTM + delta, periods);
        float priceDelta_minus0 = ZeroCouponBond(years, facevalue, YTM - delta, periods);
        float price0 = ZeroCouponBond(years, facevalue, YTM, periods);
        float duration = (priceDelta_minus0 - priceDelta_plus0) / (2 * price0 * delta);
        return duration;
    }


    //------------------------------Part D-------------------------------
    float FiniteDifferenceDuration(int years, int facevalue, float YTM, int periods, float delta, float coupon) {
        float priceDelta_plusCPN = CouponBondPrice(years, facevalue, YTM + delta, periods, coupon);
        float priceDelta_minusCPN = CouponBondPrice(years, facevalue, YTM - delta, periods, coupon);
        float priceCPN = CouponBondPrice(years, facevalue, YTM, periods, coupon);
        double duration = (priceDelta_minusCPN - priceDelta_plusCPN) / (2 * priceCPN * delta);
        return duration;
    }

    double PortfolioZeroDuration(double oneYr, double twoYr, double threeYr, double dur1, double dur2, double dur3, double price1, double price2, double price3) {
        double portfolioAbsoluteWeight = ((oneYr * price1) + (threeYr * price3) - (twoYr * price2));
        return (((((oneYr * price1) / portfolioAbsoluteWeight) * dur1) + (((threeYr * price3) / portfolioAbsoluteWeight) * dur3)) / (((twoYr * price2) / portfolioAbsoluteWeight) / dur2));
    }

    double PortDur(double oneYr, double twoYr, double threeYr, double dur1, double dur2, double dur3, double price1, double price2, double price3) {
        double portfolioAbsoluteWeight = ((oneYr * price1) + (threeYr * price3) - (twoYr * price2));
        return ((((oneYr * price1) / portfolioAbsoluteWeight) * dur1) + (((threeYr * price3) / portfolioAbsoluteWeight) * dur3) - (((twoYr * price2) / portfolioAbsoluteWeight) / dur2));
    }

};


class portfolio : public BondTypes {

public:

    //---------------------------Part F--------------------------------- see if git catches this
    float LongShortPortfolio() {
        double oneYear = ZeroCouponBond(1, 100, 0.025, 1);
        double twoYear = ZeroCouponBond(2, 100, 0.026, 1);
        double threeYear = ZeroCouponBond(3, 100, 0.027, 1);
        double portfolio = oneYear + threeYear - (2 * twoYear);
        return portfolio;
    }

    float PortfolioShockPlus100(double oneYr, double twoYr, double threeYr, double delta) {
        double oneYear = ZeroCouponBond(1, 100, (0.025 + delta), 1);
        //std::cout << oneYear << std::endl;
        double twoYear = ZeroCouponBond(2, 100, (0.026 + delta), 1);
        //std::cout << twoYear << std::endl;
        double threeYear = ZeroCouponBond(3, 100, (0.027 + delta), 1);
        //std::cout << threeYear << std::endl;
        double portfolio = oneYear + threeYear - (twoYr * twoYear);
        //std::cout << - (twoYr * twoYear) << std::endl;
        return portfolio;
    }
};


class Convexities : public BondTypes {

public:

    //---------------------------Part E---------------------------------
    float EffectiveConvexity(int years, int facevalue, float YTM, int periods, float delta, float coupon) {
        float priceDelta_plus = CouponBondPrice(years, facevalue, YTM + delta, periods, coupon);
        float priceDelta_minus = CouponBondPrice(years, facevalue, YTM - delta, periods, coupon);
        float price = CouponBondPrice(years, facevalue, YTM, periods, coupon);
        double convexity = (priceDelta_minus + priceDelta_minus - 2 * price) / (2 * price * (std::pow(delta, 2)));
        return convexity * (std::pow(delta, 2)) * 100;
    }

    float EffectiveConvexityZCPN(int years, int facevalue, float YTM, int periods, float delta) {
        float priceDelta_plus = ZeroCouponBond(years, facevalue, YTM + delta, periods);
        float priceDelta_minus = ZeroCouponBond(years, facevalue, YTM - delta, periods);
        float price = ZeroCouponBond(years, facevalue, YTM, periods);
        double convexity = (priceDelta_minus + priceDelta_minus - (2 * price)) / (2 * price * (std::pow(delta, 2)));
        return convexity * (std::pow(delta, 2)) * 100;
    }

    float PortfolioConvexity(double oneYr, double twoYr, double threeYr, float delta, float portfolio) {
        double oneYearCvx = EffectiveConvexityZCPN(1, 100, 0.025, 1, delta);
        double twoYearCvx = EffectiveConvexityZCPN(2, 100, 0.026, 1, delta);
        double threeYearCvx = EffectiveConvexityZCPN(3, 100, 0.027, 1, delta);
        double oneYear = ZeroCouponBond(1, 100, 0.025, 1);
        double twoYear = ZeroCouponBond(2, 100, 0.026, 1);
        double threeYear = ZeroCouponBond(3, 100, 0.027, 1);
        double PortfolioCvx = ((((oneYr * oneYear) / portfolio) * oneYearCvx) + (((threeYr * threeYear) / portfolio) * threeYearCvx) - (((twoYr * twoYear) / portfolio) * twoYearCvx));
        return PortfolioCvx * (std::pow(delta, 2)) * 100;
    }


};


class amortization : public BondTypes {

public:

    //------------------------------------------------- part K --------------------------------------------------
    double FiveYrCashFLows(int years, double facevalue, float YTM, int periods, double coupon, double principal) {
        double PrincipalPayment = principal * facevalue;
        double CouponPayment = coupon * facevalue;
        double BondPrice = 0.0;
        for (int n = 1; n <= years * periods; ++n) {
            std::cout << std::fixed;
            std::cout << std::setprecision(2);
            BondPrice += (PrincipalPayment + (CouponPayment / periods)) / std::pow((1 + YTM / periods), n);
            //std::cout << BondPrice << std::endl;
            if (n < 10)
                std::cout << "  " << n << "    |     " << CouponPayment + PrincipalPayment << "       |    " << (PrincipalPayment + (CouponPayment / periods)) / std::pow((1 + YTM / periods), n) << "         |      " << facevalue - ((PrincipalPayment / periods) * n) << std::endl;
            else
                std::cout << "  " << n << "    |     " << CouponPayment + PrincipalPayment << "      |    " << (PrincipalPayment + (CouponPayment / periods)) / std::pow((1 + YTM / periods), n) << "         |      " << facevalue - ((PrincipalPayment / periods) * n) << std::endl;
            CouponPayment = coupon * (facevalue - ((PrincipalPayment / (periods)*n)));  //take this out if the calcuation is messed up
        }
        return BondPrice;
    }

    double FiveYrDelta(int years, double facevalue, float YTM, int periods, double coupon, double principal) {
        double PrincipalPayment = principal * facevalue;
        double CouponPayment = coupon * facevalue;
        double BondPrice = 0.0;
        for (int n = 1; n <= years * periods; ++n) {
            BondPrice += (PrincipalPayment + (CouponPayment / periods)) / std::pow((1 + YTM / periods), n);
        }

        return BondPrice;
    }

    float FiveYrDuration(int years, double facevalue, float YTM, int periods, double coupon, double principal, double delta) {
        float priceDelta_plusCPN = FiveYrDelta(years, facevalue, YTM + delta, periods, coupon, principal);
        float priceDelta_minusCPN = FiveYrDelta(years, facevalue, YTM - delta, periods, coupon, principal);
        float priceCPN = FiveYrDelta(years, facevalue, YTM, periods, coupon, principal);
        double duration = (priceDelta_minusCPN - priceDelta_plusCPN) / (2 * priceCPN * delta);

        return duration;
    }

};


int main() {

    BondTypes bonds;
    float z1 = bonds.ZeroCouponBond(1, 100, 0.025, 1);
    float z2 = bonds.ZeroCouponBond(2, 100, 0.026, 1);
    float z3 = bonds.ZeroCouponBond(3, 100, 0.027, 1);
    std::cout << "zero coupon price : " << z1 << std::endl;

    float cpn = bonds.CouponBondPrice(1, 100, 0.025, 1, 0.03);
    std::cout << "coupon bonds :" << cpn << std::endl;

    portfolio port;
    float port1 = port.LongShortPortfolio();

    Durations duration;
    float d1 = duration.FiniteDifferenceDuration1(1, 100, 0.025, 1, 0.01);
    float d2 = duration.FiniteDifferenceDuration1(2, 100, 0.026, 1, 0.01);
    float d3 = duration.FiniteDifferenceDuration1(3, 100, 0.027, 1, 0.01);
    std::cout << "duration :" << d1 << std::endl;

    float dd2 = duration.FiniteDifferenceDuration(1, 100, 0.025, 1, 0.01, 0.03);
    std::cout << "coupon duration :" << dd2 << std::endl;

    float pd = duration.PortDur(1, 2, 1, d1, d2, d3, z1, z2, z3);
    std::cout << "portfolio duration :" << pd << std::endl;

    Convexities convex;
    float pdcvx = convex.PortfolioConvexity(1, 2, 1, 0.01, port1);
    std::cout << "portfolio convexity :" << pdcvx << std::endl;

    float pdz = duration.PortfolioZeroDuration(1, 2, 1, d1, d2, d3, z1, z2, z3);
    std::cout << "portfolio rebalanced duration :" << pdz << std::endl;

    float pdrebalnced = duration.PortDur(1, 2 * pdz, 1, d1, d2, d3, z1, z2, z3);
    std::cout << "portfolio zero duration :" << pdrebalnced << std::endl;

    float shockValuePlus = port.PortfolioShockPlus100(1, 2 * pdz, 1, 0.01);
    std::cout << "shock plus 100 bps :" << shockValuePlus << std::endl;

    float shockValueMinus = port.PortfolioShockPlus100(1, 2 * pdz, 1, -0.01);
    std::cout << "shock minus 100 bps :" << shockValueMinus << std::endl;

    amortization amort;
    std::cout << " Year  | Scheduled Value | Discounted Value | Remaining Pricipal " << std::endl;
    std::cout << " --------------------------------------------------------------- " << std::endl;
    double shmoney = amort.FiveYrCashFLows(5, 100, 0.03, 1, 0.03, 0.2);
    std::cout << std::endl;
    std::cout << "Price is : " << shmoney << std::endl;
    printf("%0.4lf", shmoney);

    std::cout << std::endl;
    double amortizationDuration = amort.FiveYrDuration(5, 100, 0.03, 1, 0.03, 0.2, 0.01);
    std::cout << "Duration is : " << amortizationDuration << std::endl;

    return 0;
}