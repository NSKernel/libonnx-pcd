#include <sys/time.h>
#include <onnx.h>

struct profiler_t {
	uint64_t begin;
	uint64_t end;
	uint64_t elapsed;
	uint64_t count;
};

uint64_t time_discrepancy;

static inline uint64_t time_get(void)
{
	struct timeval tv;

	gettimeofday(&tv, 0);
	return (uint64_t)(tv.tv_sec * 1000000000ULL + tv.tv_usec * 1000);
}

static void hmap_entry_callback(struct hmap_t * m, struct hmap_entry_t * e)
{
	if(e && e->value)
		free(e->value);
}

static struct hmap_t * profiler_alloc(int size)
{
	return hmap_alloc(size, hmap_entry_callback);
}

static void profiler_free(struct hmap_t * m)
{
	hmap_free(m);
}

static struct profiler_t * profiler_search(struct hmap_t * m, const char * name)
{
	struct profiler_t * p = NULL;

	if(m && name)
	{
		p = hmap_search(m, name);
		if(!p)
		{
			p = malloc(sizeof(struct profiler_t));
			if(p)
			{
				p->begin = 0;
				p->end = 0;
				p->elapsed = 0;
				p->count = 0;
				hmap_add(m, name, p);
			}
		}
	}
	return p;
}

static inline void profiler_begin(struct profiler_t * p)
{
	if(p)
		p->begin = time_get();
}

static inline void profiler_end(struct profiler_t * p)
{
	if(p)
	{
		p->end = time_get();
		p->elapsed += p->end - p->begin - time_discrepancy;
		p->count++;
	}
}

static void profiler_dump(struct hmap_t * m, int count)
{
	struct hmap_entry_t * e;
	struct profiler_t * p;
	double total = 0;
	double mean = 0;
	double fps = 0;

	if(m)
	{
		printf("Profiler analysis:\r\n");
		hmap_sort(m);
		hmap_for_each_entry(e, m)
		{
			p = (struct profiler_t *)e->value;
			total += p->elapsed;
			printf("%-32s %lld %12.3f(us)\r\n", e->key, p->count, (p->count > 0) ? ((double)p->elapsed / 1000.0f) / (double)p->count : 0);
		}
		if(count > 0)
		{
			mean = total / (double)count;
			fps = (double)1000000000.0 / mean;
		}
		printf("----------------------------------------------------------------\r\n");
		printf("Repeat times: %d, Average time: %.3f(us), Frame rates: %.3f(fps)\r\n", count, mean / 1000.0f, fps);
	}
}

static void onnx_run_benchmark(struct onnx_context_t * ctx, int count)
{
	struct onnx_node_t * n;
	struct hmap_t * m;
	struct profiler_t * p;
	char name[256];
	int cnt = count;
	int len, i;

	if(ctx)
	{
		m = profiler_alloc(0);
		for(i = 0; i < ctx->g->nlen; i++)
		{
			n = &ctx->g->nodes[i];
			if(n->reshape(n))
				n->operator(n);
		}
		while(count-- > 0)
		{
			for(i = 0; i < ctx->g->nlen; i++)
			{
				n = &ctx->g->nodes[i];
				len = sprintf(name, "%s-%d", n->proto->op_type, n->opset);
				len += sprintf(name + len, "%*s", 24 - len, n->r->name);
				p = profiler_search(m, name);
				profiler_begin(p);
				if(n->reshape(n))
					n->operator(n);
				profiler_end(p);
			}
		}
		profiler_dump(m, cnt);
		profiler_free(m);
	}
}

extern char *read_file_to_buffer(const char *filename, size_t *ret_size);

void builtin_free(void *ptr) __attribute__((
	__import_module__("env"),
	__import_name__("free")
));

int main()
{
	struct onnx_context_t * ctx;
	char filename[200];
	char *file_buffer;
	size_t file_size;
	int count = 0;
	int i;

	uint64_t time_diff[6];
	

	time_diff[0] = time_get();
	time_diff[1] = time_get();
	time_diff[2] = time_get();
	time_diff[3] = time_get();
	time_diff[4] = time_get();
	time_diff[5] = time_get();
	for (i = 0; i < 5; i++) {
		time_discrepancy += time_diff[i + 1] - time_diff[i];
	}
	time_discrepancy /= 5;
	printf("get time discrepancy is %llu\n", time_discrepancy);

	count = 1;
	printf("enter model name: ");
	fflush(stdout);
	scanf("%s", filename);
	file_buffer = read_file_to_buffer(filename, &file_size);
	if (file_buffer == NULL) {
		printf("failed to read from %s\n", filename);
		return -1;
	}
	printf("file size is %zu\n", file_size);
	ctx = onnx_context_alloc(file_buffer, file_size, NULL, 0);
	if(ctx)
	{
		onnx_run_benchmark(ctx, count);
		onnx_context_free(ctx);
	}
	else {
		printf("failed to allocate context\n");
	}
	builtin_free(file_buffer);
	return 0;
}
