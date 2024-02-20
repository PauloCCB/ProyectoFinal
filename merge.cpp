#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;
using namespace std::chrono;
using std::cout;
void mergeSequential(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);
    //Copiando datos a los vectores temporales L[] y R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0; 
    int j = 0; 
    int k = l; 
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeParallel(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    thread t1([&arr, &L, &n1, &R, &n2, &l]() {
        int i = 0, j = 0, k = l;
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                arr[k] = L[i];
                i++;
            }
            else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }
    });

    thread t2([&arr, &L, &n1, &R, &n2, &l, &m, &r]() {
        int i = n1 - 1, j = n2 - 1, k = r;
        while (i >= 0 && j >= 0) {
            if (L[i] >= R[j]) {
                arr[k] = L[i];
                i--;
            }
            else {
                arr[k] = R[j];
                j--;
            }
            k--;
        }

        while (j >= 0) {
            arr[k] = R[j];
            j--;
            k--;
        }
    });
    t1.join();
    t2.join();
}

void mergeSortSecuencial
(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortSecuencial
        (arr, l, m);
        mergeSortSecuencial
        (arr, m + 1, r);
        mergeSequential(arr, l, m, r);
    }
}

void mergeSortParallel(vector<int>& arr, int l, int r, int profundidad) {
    if (l < r) {
        if (profundidad <= 0) {
            mergeSortSecuencial
            (arr, l, r);
            return;
        }
        
        int m = l + (r - l) / 2;
        thread t1(mergeSortParallel, ref(arr), l, m, profundidad - 1);
        thread t2(mergeSortParallel, ref(arr), m + 1, r, profundidad - 1);
        t1.join();
        t2.join();

        mergeParallel(arr, l, m, r);
    }
}

int main() {
    vector<int> arr = { 151, 719, 833, 726, 664, 799, 556, 122, 236, 382, 223, 610, 413, 983, 299, 401, 
    296, 140, 517, 989,
    647, 546, 975, 987, 542, 669, 207, 792, 190, 129, 516, 714, 209, 401, 431, 113, 599, 533, 573, 627,
    336, 675, 501, 204, 383, 441, 855, 409, 644, 834, 730, 854, 532, 296, 284, 349, 298, 320, 932, 352,
    687, 734, 938, 509, 232, 937, 398, 684, 651, 231, 926, 402, 179, 114, 893, 690, 692, 987, 555, 661,
    833, 249, 571, 420, 896, 249, 426, 874, 476, 830, 743, 450, 846, 844, 228, 424, 218, 318, 647, 671,
    819, 432, 269, 554, 513, 175, 268, 242, 694, 244, 289, 360, 700, 122, 250, 771, 725, 354, 429, 453,
    936, 692, 656, 987, 516, 308, 690, 949, 480, 701, 200, 166, 548, 884, 543, 241, 537, 835, 675, 607,
    450, 174, 651, 571, 307, 268, 655, 368, 734, 213, 278, 384, 146, 422, 408, 481, 892, 328, 541, 499,
    768, 433, 354, 499, 874, 221, 675, 227, 797, 532, 785, 316, 163, 750, 206, 648, 931, 760, 541, 758,
    643, 190, 512, 547, 317, 586, 210, 601, 785, 314, 231, 303, 186, 387, 874, 179, 754, 222, 588, 179,
    784, 381, 647, 119, 921, 685, 443, 618, 905, 128, 139, 420, 896, 803, 157, 432, 162, 720, 872, 422,
    940, 917, 792, 805, 708, 627, 467, 894, 692, 748, 792, 781, 214, 906, 190, 768, 518, 375, 432, 241,
    441, 424, 782, 382, 832, 699, 685, 699, 109, 810, 201, 262, 769, 878, 150, 216, 327, 627, 446, 719,
    315, 539, 731, 921, 683, 732, 256, 949, 132, 886, 948, 501, 255, 375, 889, 699, 272, 655, 529, 943,
    291, 233, 340, 274, 456, 109, 665, 542, 113, 771, 392, 188, 651, 706, 563, 756, 390, 671, 723, 915,
    360, 583, 826, 831, 430, 364, 892, 518, 489, 494, 569, 367, 291, 748, 761, 444, 578, 319, 258, 760,
    759, 801, 318, 809, 765, 120, 788, 821, 422, 294, 759, 252, 305, 283, 374, 198, 108, 565, 879, 198,
    779, 495, 676, 792, 877, 707, 106, 211, 659, 801, 357, 688, 281, 873, 211, 803, 214, 263, 926, 159,
    907, 593, 612, 619, 253, 972, 448, 790, 465, 548, 285, 415, 155, 676, 735, 929, 844, 726, 619, 109,
    378, 503, 601, 444, 934, 564, 784, 312, 970, 278, 370, 723, 623, 664, 906, 414, 227, 150, 607, 282,
    855, 440, 927, 587, 194, 638, 885, 138, 829, 674, 481, 211, 902, 813, 566, 516, 975, 999, 620, 796,
    200, 235, 809, 515, 876, 169, 991, 313, 621, 283, 824, 831, 830, 228, 275, 207, 286, 543, 134, 833,
    803, 194, 987, 875, 496, 176, 315, 423, 525, 745, 651, 993, 422, 201, 222, 578, 981, 296, 332, 899,
    367, 225, 396, 788, 872, 280, 289, 372, 361, 563, 694, 415, 467, 832, 229, 987, 256, 252, 438, 763,
    970, 256, 785, 725, 473, 804, 165, 150, 951, 883, 893, 199, 747, 423, 151, 719, 833, 726, 664, 799, 556, 122, 236, 382, 223, 610, 413, 983, 299, 401, 296, 140, 517, 989,
    647, 546, 975, 987, 542, 669, 207, 792, 190, 129, 516, 714, 209, 401, 431, 113, 599, 533, 573, 627,
    336, 675, 501, 204, 383, 441, 855, 409, 644, 834, 730, 854, 532, 296, 284, 349, 298, 320, 932, 352,
    687, 734, 938, 509, 232, 937, 398, 684, 651, 231, 926, 402, 179, 114, 893, 690, 692, 987, 555, 661,
    833, 249, 571, 420, 896, 249, 426, 874, 476, 830, 743, 450, 846, 844, 228, 424, 218, 318, 647, 671,
    819, 432, 269, 554, 513, 175, 268, 242, 694, 244, 289, 360, 700, 122, 250, 771, 725, 354, 429, 453,
    936, 692, 656, 987, 516, 308, 690, 949, 480, 701, 200, 166, 548, 884, 543, 241, 537, 835, 675, 607,
    450, 174, 651, 571, 307, 268, 655, 368, 734, 213, 278, 384, 146, 422, 408, 481, 892, 328, 541, 499,
    768, 433, 354, 499, 874, 221, 675, 227, 797, 532, 785, 316, 163, 750, 206, 648, 931, 760, 541, 758,
    643, 190, 512, 547, 317, 586, 210, 601, 785, 314, 231, 303, 186, 387, 874, 179, 754, 222, 588, 179,
    784, 381, 647, 119, 921, 685, 443, 618, 905, 128, 139, 420, 896, 803, 157, 432, 162, 720, 872, 422,
    940, 917, 792, 805, 708, 627, 467, 894, 692, 748, 792, 781, 214, 906, 190, 768, 518, 375, 432, 241,
    441, 424, 782, 382, 832, 699, 685, 699, 109, 810, 201, 262, 769, 878, 150, 216, 327, 627, 446, 719,
    315, 539, 731, 921, 683, 732, 256, 949, 132, 886, 948, 501, 255, 375, 889, 699, 272, 655, 529, 943,
    291, 233, 340, 274, 456, 109, 665, 542, 113, 771, 392, 188, 651, 706, 563, 756, 390, 671, 723, 915,
    360, 583, 826, 831, 430, 364, 892, 518, 489, 494, 569, 367, 291, 748, 761, 444, 578, 319, 258, 760,
    759, 801, 318, 809, 765, 120, 788, 821, 422, 294, 759, 252, 305, 283, 374, 198, 108, 565, 879, 198,
    779, 495, 676, 792, 877, 707, 106, 211, 659, 801, 357, 688, 281, 873, 211, 803, 214, 263, 926, 159,
    907, 593, 612, 619, 253, 972, 448, 790, 465, 548, 285, 415, 155, 676, 735, 929, 844, 726, 619, 109,
    378, 503, 601, 444, 934, 564, 784, 312, 970, 278, 370, 723, 623, 664, 906, 414, 227, 150, 607, 282,
    855, 440, 927, 587, 194, 638, 885, 138, 829, 674, 481, 211, 902, 813, 566, 516, 975, 999, 620, 796,
    200, 235, 809, 515, 876, 169, 991, 313, 621, 283, 824, 831, 830, 228, 275, 207, 286, 543, 134, 833,
    803, 194, 987, 875, 496, 176, 315, 423, 525, 745, 651, 993, 422, 201, 222, 578, 981, 296, 332, 899,
    367, 225, 396, 788, 872, 280, 289, 372, 361, 563, 694, 415, 467, 832, 229, 987, 256, 252, 438, 763,
    970, 256, 785, 725, 473, 804, 165, 150, 951, 883, 893, 199, 747, 423,  151, 719, 833, 726, 664, 799, 
    556, 122, 236, 382, 223, 610, 413, 983, 299, 401, 296, 140, 517, 989,
    647, 546, 975, 987, 542, 669, 207, 792, 190, 129, 516, 714, 209, 401, 431, 113, 599, 533, 573, 627,
    336, 675, 501, 204, 383, 441, 855, 409, 644, 834, 730, 854, 532, 296, 284, 349, 298, 320, 932, 352,
    687, 734, 938, 509, 232, 937, 398, 684, 651, 231, 926, 402, 179, 114, 893, 690, 692, 987, 555, 661,
    833, 249, 571, 420, 896, 249, 426, 874, 476, 830, 743, 450, 846, 844, 228, 424, 218, 318, 647, 671,
    819, 432, 269, 554, 513, 175, 268, 242, 694, 244, 289, 360, 700, 122, 250, 771, 725, 354, 429, 453,
    936, 692, 656, 987, 516, 308, 690, 949, 480, 701, 200, 166, 548, 884, 543, 241, 537, 835, 675, 607,
    450, 174, 651, 571, 307, 268, 655, 368, 734, 213, 278, 384, 146, 422, 408, 481, 892, 328, 541, 499,
    768, 433, 354, 499, 874, 221, 675, 227, 797, 532, 785, 316, 163, 750, 206, 648, 931, 760, 541, 758,
    643, 190, 512, 547, 317, 586, 210, 601, 785, 314, 231, 303, 186, 387, 874, 179, 754, 222, 588, 179,
    784, 381, 647, 119, 921, 685, 443, 618, 905, 128, 139, 420, 896, 803, 157, 432, 162, 720, 872, 422,
    940, 917, 792, 805, 708, 627, 467, 894, 692, 748, 792, 781, 214, 906, 190, 768, 518, 375, 432, 241,
    441, 424, 782, 382, 832, 699, 685, 699, 109, 810, 201, 262, 769, 878, 150, 216, 327, 627, 446, 719,
    315, 539, 731, 921, 683, 732, 256, 949, 132, 886, 948, 501, 255, 375, 889, 699, 272, 655, 529, 943,
    291, 233, 340, 274, 456, 109, 665, 542, 113, 771, 392, 188, 651, 706, 563, 756, 390, 671, 723, 915,
    360, 583, 826, 831, 430, 364, 892, 518, 489, 494, 569, 367, 291, 748, 761, 444, 578, 319, 258, 760,
    759, 801, 318, 809, 765, 120, 788, 821, 422, 294, 759, 252, 305, 283, 374, 198, 108, 565, 879, 198,
    779, 495, 676, 792, 877, 707, 106, 211, 659, 801, 357, 688, 281, 873, 211, 803, 214, 263, 926, 159,
    907, 593, 612, 619, 253, 972, 448, 790, 465, 548, 285, 415, 155, 676, 735, 929, 844, 726, 619, 109,
    378, 503, 601, 444, 934, 564, 784, 312, 970, 278, 370, 723, 623, 664, 906, 414, 227, 150, 607, 282,
    855, 440, 927, 587, 194, 638, 885, 138, 829, 674, 481, 211, 902, 813, 566, 516, 975, 999, 620, 796,
    200, 235, 809, 515, 876, 169, 991, 313, 621, 283, 824, 831, 830, 228, 275, 207, 286, 543, 134, 833,
    803, 194, 987, 875, 496, 176, 315, 423, 525, 745, 651, 993, 422, 201, 222, 578, 981, 296, 332, 899,
    367, 225, 396, 788, 872, 280, 289, 372, 361, 563, 694, 415, 467, 832, 229, 987, 256, 252, 438, 763,
    970, 256, 785, 725, 473, 804, 165, 150, 951, 883, 893, 199, 747, 423,  151, 719, 833, 726, 664, 799, 
    556, 122, 236, 382, 223, 610, 413, 983, 299, 401, 296, 140, 517, 989,
    647, 546, 975, 987, 542, 669, 207, 792, 190, 129, 516, 714, 209, 401, 431, 113, 599, 533, 573, 627,
    336, 675, 501, 204, 383, 441, 855, 409, 644, 834, 730, 854, 532, 296, 284, 349, 298, 320, 932, 352,
    687, 734, 938, 509, 232, 937, 398, 684, 651, 231, 926, 402, 179, 114, 893, 690, 692, 987, 555, 661,
    833, 249, 571, 420, 896, 249, 426, 874, 476, 830, 743, 450, 846, 844, 228, 424, 218, 318, 647, 671,
    819, 432, 269, 554, 513, 175, 268, 242, 694, 244, 289, 360, 700, 122, 250, 771, 725, 354, 429, 453,
    936, 692, 656, 987, 516, 308, 690, 949, 480, 701, 200, 166, 548, 884, 543, 241, 537, 835, 675, 607,
    450, 174, 651, 571, 307, 268, 655, 368, 734, 213, 278, 384, 146, 422, 408, 481, 892, 328, 541, 499,
    768, 433, 354, 499, 874, 221, 675, 227, 797, 532, 785, 316, 163, 750, 206, 648, 931, 760, 541, 758,
    643, 190, 512, 547, 317, 586, 210, 601, 785, 314, 231, 303, 186, 387, 874, 179, 754, 222, 588, 179,
    784, 381, 647, 119, 921, 685, 443, 618, 905, 128, 139, 420, 896, 803, 157, 432, 162, 720, 872, 422,
    940, 917, 792, 805, 708, 627, 467, 894, 692, 748, 792, 781, 214, 906, 190, 768, 518, 375, 432, 241,
    441, 424, 782, 382, 832, 699, 685, 699, 109, 810, 201, 262, 769, 878, 150, 216, 327, 627, 446, 719,
    315, 539, 731, 921, 683, 732, 256, 949, 132, 886, 948, 501, 255, 375, 889, 699, 272, 655, 529, 943,
    291, 233, 340, 274, 456, 109, 665, 542, 113, 771, 392, 188, 651, 706, 563, 756, 390, 671, 723, 915,
    360, 583, 826, 831, 430, 364, 892, 518, 489, 494, 569, 367, 291, 748, 761, 444, 578, 319, 258, 760,
    759, 801, 318, 809, 765, 120, 788, 821, 422, 294, 759, 252, 305, 283, 374, 198, 108, 565, 879, 198,
    779, 495, 676, 792, 877, 707, 106, 211, 659, 801, 357, 688, 281, 873, 211, 803, 214, 263, 926, 159,
    907, 593, 612, 619, 253, 972, 448, 790, 465, 548, 285, 415, 155, 676, 735, 929, 844, 726, 619, 109,
    378, 503, 601, 444, 934, 564, 784, 312, 970, 278, 370, 723, 623, 664, 906, 414, 227, 150, 607, 282,
    855, 440, 927, 587, 194, 638, 885, 138, 829, 674, 481, 211, 902, 813, 566, 516, 975, 999, 620, 796,
    200, 235, 809, 515, 876, 169, 991, 313, 621, 283, 824, 831, 830, 228, 275, 207, 286, 543, 134, 833,
    803, 194, 987, 875, 496, 176, 315, 423, 525, 745, 651, 993, 422, 201, 222, 578, 981, 296, 332, 899,
    367, 225, 396, 788, 872, 280, 289, 372, 361, 563, 694, 415, 467, 832, 229, 987, 256, 252, 438, 763,
    970, 256, 785, 725, 473, 804, 165, 150, 951, 883, 893, 199, 747, 423,  151, 719, 833, 726, 664, 799, 556, 122, 236, 382, 223, 610, 413, 983, 299, 401, 296, 140, 517, 989,
    647, 546, 975, 987, 542, 669, 207, 792, 190, 129, 516, 714, 209, 401, 431, 113, 599, 533, 573, 627,
    336, 675, 501, 204, 383, 441, 855, 409, 644, 834, 730, 854, 532, 296, 284, 349, 298, 320, 932, 352,
    687, 734, 938, 509, 232, 937, 398, 684, 651, 231, 926, 402, 179, 114, 893, 690, 692, 987, 555, 661,
    833, 249, 571, 420, 896, 249, 426, 874, 476, 830, 743, 450, 846, 844, 228, 424, 218, 318, 647, 671,
    819, 432, 269, 554, 513, 175, 268, 242, 694, 244, 289, 360, 700, 122, 250, 771, 725, 354, 429, 453,
    936, 692, 656, 987, 516, 308, 690, 949, 480, 701, 200, 166, 548, 884, 543, 241, 537, 835, 675, 607,
    450, 174, 651, 571, 307, 268, 655, 368, 734, 213, 278, 384, 146, 422, 408, 481, 892, 328, 541, 499,
    768, 433, 354, 499, 874, 221, 675, 227, 797, 532, 785, 316, 163, 750, 206, 648, 931, 760, 541, 758,
    643, 190, 512, 547, 317, 586, 210, 601, 785, 314, 231, 303, 186, 387, 874, 179, 754, 222, 588, 179,
    784, 381, 647, 119, 921, 685, 443, 618, 905, 128, 139, 420, 896, 803, 157, 432, 162, 720, 872, 422,
    940, 917, 792, 805, 708, 627, 467, 894, 692, 748, 792, 781, 214, 906, 190, 768, 518, 375, 432, 241,
    441, 424, 782, 382, 832, 699, 685, 699, 109, 810, 201, 262, 769, 878, 150, 216, 327, 627, 446, 719,
    315, 539, 731, 921, 683, 732, 256, 949, 132, 886, 948, 501, 255, 375, 889, 699, 272, 655, 529, 943,
    291, 233, 340, 274, 456, 109, 665, 542, 113, 771, 392, 188, 651, 706, 563, 756, 390, 671, 723, 915,
    360, 583, 826, 831, 430, 364, 892, 518, 489, 494, 569, 367, 291, 748, 761, 444, 578, 319, 258, 760,
    759, 801, 318, 809, 765, 120, 788, 821, 422, 294, 759, 252, 305, 283, 374, 198, 108, 565, 879, 198,
    779, 495, 676, 792, 877, 707, 106, 211, 659, 801, 357, 688, 281, 873, 211, 803, 214, 263, 926, 159,
    907, 593, 612, 619, 253, 972, 448, 790, 465, 548, 285, 415, 155, 676, 735, 929, 844, 726, 619, 109,
    378, 503, 601, 444, 934, 564, 784, 312, 970, 278, 370, 723, 623, 664, 906, 414, 227, 150, 607, 282,
    855, 440, 927, 587, 194, 638, 885, 138, 829, 674, 481, 211, 902, 813, 566, 516, 975, 999, 620, 796,
    200, 235, 809, 515, 876, 169, 991, 313, 621, 283, 824, 831, 830, 228, 275, 207, 286, 543, 134, 833,
    803, 194, 987, 875, 496, 176, 315, 423, 525, 745, 651, 993, 422, 201, 222, 578, 981, 296, 332, 899,
    367, 225, 396, 788, 872, 280, 289, 372, 361, 563, 694, 415, 467, 832, 229, 987, 256, 252, 438, 763,
    970, 256, 785, 725, 473, 804, 165, 150, 951, 883, 893, 199, 747, 423,  151, 719, 833, 726, 664, 799, 556, 122, 236, 382, 223, 610, 413, 983, 299, 401, 296, 140, 517, 989,
    647, 546, 975, 987, 542, 669, 207, 792, 190, 129, 516, 714, 209, 401, 431, 113, 599, 533, 573, 627,
    336, 675, 501, 204, 383, 441, 855, 409, 644, 834, 730, 854, 532, 296, 284, 349, 298, 320, 932, 352,
    687, 734, 938, 509, 232, 937, 398, 684, 651, 231, 926, 402, 179, 114, 893, 690, 692, 987, 555, 661,
    833, 249, 571, 420, 896, 249, 426, 874, 476, 830, 743, 450, 846, 844, 228, 424, 218, 318, 647, 671,
    819, 432, 269, 554, 513, 175, 268, 242, 694, 244, 289, 360, 700, 122, 250, 771, 725, 354, 429, 453,
    936, 692, 656, 987, 516, 308, 690, 949, 480, 701, 200, 166, 548, 884, 543, 241, 537, 835, 675, 607,
    450, 174, 651, 571, 307, 268, 655, 368, 734, 213, 278, 384, 146, 422, 408, 481, 892, 328, 541, 499,
    768, 433, 354, 499, 874, 221, 675, 227, 797, 532, 785, 316, 163, 750, 206, 648, 931, 760, 541, 758,
    643, 190, 512, 547, 317, 586, 210, 601, 785, 314, 231, 303, 186, 387, 874, 179, 754, 222, 588, 179,
    784, 381, 647, 119, 921, 685, 443, 618, 905, 128, 139, 420, 896, 803, 157, 432, 162, 720, 872, 422,
    940, 917, 792, 805, 708, 627, 467, 894, 692, 748, 792, 781, 214, 906, 190, 768, 518, 375, 432, 241,
    441, 424, 782, 382, 832, 699, 685, 699, 109, 810, 201, 262, 769, 878, 150, 216, 327, 627, 446, 719,
    315, 539, 731, 921, 683, 732, 256, 949, 132, 886, 948, 501, 255, 375, 889, 699, 272, 655, 529, 943,
    291, 233, 340, 274, 456, 109, 665, 542, 113, 771, 392, 188, 651, 706, 563, 756, 390, 671, 723, 915,
    360, 583, 826, 831, 430, 364, 892, 518, 489, 494, 569, 367, 291, 748, 761, 444, 578, 319, 258, 760,
    759, 801, 318, 809, 765, 120, 788, 821, 422, 294, 759, 252, 305, 283, 374, 198, 108, 565, 879, 198,
    779, 495, 676, 792, 877, 707, 106, 211, 659, 801, 357, 688, 281, 873, 211, 803, 214, 263, 926, 159,
    907, 593, 612, 619, 253, 972, 448, 790, 465, 548, 285, 415, 155, 676, 735, 929, 844, 726, 619, 109,
    378, 503, 601, 444, 934, 564, 784, 312, 970, 278, 370, 723, 623, 664, 906, 414, 227, 150, 607, 282,
    855, 440, 927, 587, 194, 638, 885, 138, 829, 674, 481, 211, 902, 813, 566, 516, 975, 999, 620, 796,
    200, 235, 809, 515, 876, 169, 991, 313, 621, 283, 824, 831, 830, 228, 275, 207, 286, 543, 134, 833,
    803, 194, 987, 875, 496, 176, 315, 423, 525, 745, 651, 993, 422, 201, 222, 578, 981, 296, 332, 899,
    367, 225, 396, 788, 872, 280, 289, 372, 361, 563, 694, 415, 467, 832, 229, 987, 256, 252, 438, 763,
    970, 256, 785, 725, 473, 804, 165, 150, 951, 883, 893, 199, 747, 423,   151, 719, 833, 726, 664, 799, 556, 122, 236, 382, 223, 610, 413, 983, 299, 401, 296, 140, 517, 989,
    647, 546, 975, 987, 542, 669, 207, 792, 190, 129, 516, 714, 209, 401, 431, 113, 599, 533, 573, 627,
    336, 675, 501, 204, 383, 441, 855, 409, 644, 834, 730, 854, 532, 296, 284, 349, 298, 320, 932, 352,
    687, 734, 938, 509, 232, 937, 398, 684, 651, 231, 926, 402, 179, 114, 893, 690, 692, 987, 555, 661,
    833, 249, 571, 420, 896, 249, 426, 874, 476, 830, 743, 450, 846, 844, 228, 424, 218, 318, 647, 671,
    819, 432, 269, 554, 513, 175, 268, 242, 694, 244, 289, 360, 700, 122, 250, 771, 725, 354, 429, 453,
    936, 692, 656, 987, 516, 308, 690, 949, 480, 701, 200, 166, 548, 884, 543, 241, 537, 835, 675, 607,
    450, 174, 651, 571, 307, 268, 655, 368, 734, 213, 278, 384, 146, 422, 408, 481, 892, 328, 541, 499,
    768, 433, 354, 499, 874, 221, 675, 227, 797, 532, 785, 316, 163, 750, 206, 648, 931, 760, 541, 758,
    643, 190, 512, 547, 317, 586, 210, 601, 785, 314, 231, 303, 186, 387, 874, 179, 754, 222, 588, 179,
    784, 381, 647, 119, 921, 685, 443, 618, 905, 128, 139, 420, 896, 803, 157, 432, 162, 720, 872, 422,
    940, 917, 792, 805, 708, 627, 467, 894, 692, 748, 792, 781, 214, 906, 190, 768, 518, 375, 432, 241,
    441, 424, 782, 382, 832, 699, 685, 699, 109, 810, 201, 262, 769, 878, 150, 216, 327, 627, 446, 719,
    315, 539, 731, 921, 683, 732, 256, 949, 132, 886, 948, 501, 255, 375, 889, 699, 272, 655, 529, 943,
    291, 233, 340, 274, 456, 109, 665, 542, 113, 771, 392, 188, 651, 706, 563, 756, 390, 671, 723, 915,
    360, 583, 826, 831, 430, 364, 892, 518, 489, 494, 569, 367, 291, 748, 761, 444, 578, 319, 258, 760,
    759, 801, 318, 809, 765, 120, 788, 821, 422, 294, 759, 252, 305, 283, 374, 198, 108, 565, 879, 198,
    779, 495, 676, 792, 877, 707, 106, 211, 659, 801, 357, 688, 281, 873, 211, 803, 214, 263, 926, 159,
    907, 593, 612, 619, 253, 972, 448, 790, 465, 548, 285, 415, 155, 676, 735, 929, 844, 726, 619, 109,
    378, 503, 601, 444, 934, 564, 784, 312, 970, 278, 370, 723, 623, 664, 906, 414, 227, 150, 607, 282,
    855, 440, 927, 587, 194, 638, 885, 138, 829, 674, 481, 211, 902, 813, 566, 516, 975, 999, 620, 796,
    200, 235, 809, 515, 876, 169, 991, 313, 621, 283, 824, 831, 830, 228, 275, 207, 286, 543, 134, 833,
    803, 194, 987, 875, 496, 176, 315, 423, 525, 745, 651, 993, 422, 201, 222, 578, 981, 296, 332, 899,
    367, 225, 396, 788, 872, 280, 289, 372, 361, 563, 694, 415, 467, 832, 229, 987, 256, 252, 438, 763,
    970, 256, 785, 725, 473, 804, 165, 150, 951, 883, 893, 199, 747, 423,  151, 719, 833, 726, 664, 799, 556, 122, 236, 382, 223, 610, 413, 983, 299, 401, 296, 140, 517, 989,
    647, 546, 975, 987, 542, 669, 207, 792, 190, 129, 516, 714, 209, 401, 431, 113, 599, 533, 573, 627,
    336, 675, 501, 204, 383, 441, 855, 409, 644, 834, 730, 854, 532, 296, 284, 349, 298, 320, 932, 352,
    687, 734, 938, 509, 232, 937, 398, 684, 651, 231, 926, 402, 179, 114, 893, 690, 692, 987, 555, 661,
    833, 249, 571, 420, 896, 249, 426, 874, 476, 830, 743, 450, 846, 844, 228, 424, 218, 318, 647, 671,
    819, 432, 269, 554, 513, 175, 268, 242, 694, 244, 289, 360, 700, 122, 250, 771, 725, 354, 429, 453,
    936, 692, 656, 987, 516, 308, 690, 949, 480, 701, 200, 166, 548, 884, 543, 241, 537, 835, 675, 607,
    450, 174, 651, 571, 307, 268, 655, 368, 734, 213, 278, 384, 146, 422, 408, 481, 892, 328, 541, 499,
    768, 433, 354, 499, 874, 221, 675, 227, 797, 532, 785, 316, 163, 750, 206, 648, 931, 760, 541, 758,
    643, 190, 512, 547, 317, 586, 210, 601, 785, 314, 231, 303, 186, 387, 874, 179, 754, 222, 588, 179,
    784, 381, 647, 119, 921, 685, 443, 618, 905, 128, 139, 420, 896, 803, 157, 432, 162, 720, 872, 422,
    940, 917, 792, 805, 708, 627, 467, 894, 692, 748, 792, 781, 214, 906, 190, 768, 518, 375, 432, 241,
    441, 424, 782, 382, 832, 699, 685, 699, 109, 810, 201, 262, 769, 878, 150, 216, 327, 627, 446, 719,
    315, 539, 731, 921, 683, 732, 256, 949, 132, 886, 948, 501, 255, 375, 889, 699, 272, 655, 529, 943,
    291, 233, 340, 274, 456, 109, 665, 542, 113, 771, 392, 188, 651, 706, 563, 756, 390, 671, 723, 915,
    360, 583, 826, 831, 430, 364, 892, 518, 489, 494, 569, 367, 291, 748, 761, 444, 578, 319, 258, 760,
    759, 801, 318, 809, 765, 120, 788, 821, 422, 294, 759, 252, 305, 283, 374, 198, 108, 565, 879, 198,
    779, 495, 676, 792, 877, 707, 106, 211, 659, 801, 357, 688, 281, 873, 211, 803, 214, 263, 926, 159,
    907, 593, 612, 619, 253, 972, 448, 790, 465, 548, 285, 415, 155, 676, 735, 929, 844, 726, 619, 109,
    378, 503, 601, 444, 934, 564, 784, 312, 970, 278, 370, 723, 623, 664, 906, 414, 227, 150, 607, 282,
    855, 440, 927, 587, 194, 638, 885, 138, 829, 674, 481, 211, 902, 813, 566, 516, 975, 999, 620, 796,
    200, 235, 809, 515, 876, 169, 991, 313, 621, 283, 824, 831, 830, 228, 275, 207, 286, 543, 134, 833,
    803, 194, 987, 875, 496, 176, 315, 423, 525, 745, 651, 993, 422, 201, 222, 578, 981, 296, 332, 899,
    367, 225, 396, 788, 872, 280, 289, 372, 361, 563, 694, 415, 467, 832, 229, 987, 256, 252, 438, 763,
    970, 256, 785, 725, 473, 804, 165, 150, 951, 883, 893, 199, 747, 423};
    int arr_size = arr.size();

    cout << "Vector original:" << endl;
    for (int i = 0; i < arr_size; i++)
        cout << arr[i] << " ";
    cout << endl;
    // Inicia el conteo .
    auto startSequential = high_resolution_clock::now();
    mergeSortSecuencial
    (arr, 0, arr_size - 1);
     // Finaliza el conteo después de ejecutar el algoritmo.
    auto stopSequential = high_resolution_clock::now();
     // Se calcula la duración total de la ejecución restando los tiempos de inicio y finalización.
    auto durationSequential = duration_cast<microseconds>(stopSequential - startSequential);

    cout << "Vector ordenado secuencialmente:" << endl;
    for (int i = 0; i < arr_size; i++)
        cout << arr[i] << " ";
    cout << endl;

    vector<int> arr_parallel = arr;  

    auto startParallel = high_resolution_clock::now();
    mergeSortParallel(arr_parallel, 0, arr_size - 1, 2);
    auto stopParallel = high_resolution_clock::now();
    auto durationParallel = duration_cast<microseconds>(stopParallel - startParallel);

    cout << "Vector ordenado con paralelismo:" << endl;
    for (int i = 0; i < arr_size; i++)
        cout << arr_parallel[i] << " ";
    cout << endl;
    cout << "Tiempo secuencial: " << durationSequential.count() << " microseconds" << endl;
    cout << "Tiempo paralelo: " << durationParallel.count() << " microseconds" << endl;

    return 0;
}