typedef struct {
	int mantissa;
	int8_t exponent;
} floating_point;

void fp_set(floating_point *result, int mantissa, int8_t exponent)
{
	result->mantissa = mantissa;
	result->exponent = exponent;
}

void fp_set_int(floating_point *dest, int src)
{
	dest->mantissa = src;
	dest->exponent = 0;
}

void fp_set_positive_infinity(floating_point *f)
{
	f->mantissa = INT_MAX;
	f->exponent = INT8_MAX;
}

void fp_add(floating_point *result, floating_point *additional)
{
	result->mantissa += additional->mantissa;
}

void fp_subtract(floating_point *result, floating_point *less)
{
	result->mantissa -= less->mantissa;
}

int fp_trunc_to_int(floating_point *f)
{
	int8_t i;
	int result;

	result = f->mantissa;
	for (i = 0; i < f->exponent; ++i) {
		result *= 10;
	}
	return result;
}

void fp_print(floating_point *f)
{
	printf("%dE%d", f->mantissa, f->exponent);
}

