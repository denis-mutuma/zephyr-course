/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */


#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"
#include "zephyr/sys/rb.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	struct rb;

	zassert_ok(rb_init(4), "rb init(4) should succeed");

	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	/* TODO(l8-task1): Push a value, call rb_init(4) again, then
	 * verify the buffer is empty and count is 0.
	 * See TEST_SPEC.md "Suite ring_buf_init" #2.
	 */

	struct rb;

	zassert_ok(rb_init(4), "rb_init(4) should succeed");
	zassert_ok(rb_push(10), "rb_push(10) should succeed");

	zassert_ok(rb_init(4), "rb_init(4) should succeed again");
	zassert_true(rb_is_empty(), "Buffer should be empty after re-init");
	zassert_equal(rb_count(), 0, "Buffer count should be 0 after re-init");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	/* TODO(l8-task1): rb_push(42), rb_pop(&v) -> v == 42, buffer empty after.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #1.
	 */

	struct rb;
	int v;

	zassert_ok(rb_init(4), "rb_init(4) should succeed");
	zassert_ok(rb_push(42), "rb_push(42) should succeed");
	zassert_ok(rb_pop(&v), "rb_pop(&v) should succeed");
	zassert_equal(v, 42, "Popped value should be 42");
	zassert_true(rb_is_empty(), "Buffer should be empty after pop");
	zassert_equal(rb_count(), 0, "Buffer count should be 0 after pop");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	/* TODO(l8-task1): rb_push(1), rb_push(2), rb_push(3); pop three times
	 * and verify the values come out as 1, 2, 3 in that order.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #2.
	 */

	struct rb;
	int v;

	zassert_ok(rb_init(4), "rb_init(4) should succeed");
	zassert_ok(rb_push(1), "rb_push(1) should succeed");
	zassert_ok(rb_push(2), "rb_push(2) should succeed");
	zassert_ok(rb_push(3), "rb_push(3) should succeed");
	zassert_equal(rb_count(), 3, "ring buffer should contain three items");
	
	zassert_ok(rb_pop(&v), "rb_pop() should succeed");
	zassert_equal(v,1, "The ring buffer is a FIFO queue and the first pop should remove the integer 1");
	zassert_equal(rb_count(), 2, "The buffer should contain two elements after popping once");

	zassert_ok(rb_pop(&v), "The buffer has two elements and rb_pop() should succeed");
	zassert_equal(v,2, "v should be 2 because this is a FIFO data structure");
	zassert_equal(rb_count(), 1, "The buffer should now contain one element after popping twice");

	zassert_ok(rb_pop(&v), "poping from a non-empty ring buffer should succeed");
	zassert_equal(v,3, "This is a FIFO ring buffer and three should be the latest popped item");
	zassert_equal(rb_count(), 0, "The ring buffer should contain no elements");
	zassert_true(rb_is_empty(), "The buffer should be empty");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	/* TODO(l8-task1): Fill the buffer to its capacity of 4, then push
	 * one more value -> -ENOSPC.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #3.
	 */
	struct rb;

	zassert_ok(rb_init(4), "rb_init(4) should succeed");
	zassert_ok(rb_push(11), "rb_push(11) should succeed");
	zassert_ok(rb_push(22), "rb_push(22) should succeed");
	zassert_ok(rb_push(33), "rb_push(33) should succeed");
	zassert_ok(rb_push(44), "rb_push(44) should succeed");
	zassert_equal(rb_push(55), -ENOSPC, "rb_push(55) should fail and return -ENOSPC");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	/* TODO(l8-task1): rb_push(7); rb_peek(&v) -> v == 7; rb_peek(&v) again
	 * -> v == 7; rb_count() still == 1.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #1.
	 */
	
	 struct rb;
	 int v;

	 zassert_ok(rb_init(4), "rb_init(4) should succeed");
	 zassert_ok(rb_push(7), "rb_push(7) should succeed");
	 zassert_ok(rb_peek(&v), "rb_peek(&v) should succeed");
	 zassert_equal(v, 7, "v should contain the value 7");
	 zassert_equal(rb_count(), 1, "rb_peek(&v) should not modify the ring buffer");
	 zassert_ok(rb_peek(&v), "rb_peek(&v) should succeed");
	 zassert_equal(rb_count(), 1, "calling rb_peek(&v) multiple times should not modify the ring buffer");
	 zassert_equal(v, 7, "calling rb_peek(&v) multiple times should return the same value");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	/* TODO(l8-task1): rb_pop(NULL) -> -EINVAL.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #2.
	 */
	
	 struct rb;

	 zassert_ok(rb_init(4), "rb_init(4) should succeed");
	 zassert_ok(rb_push(7), "rb_push(7) should succeed");
	 zassert_equal(rb_pop(NULL), -EINVAL, "should return -EINVAL if v is null");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	/* TODO(l8-task1): push 4 values -> rb_is_full() == true, rb_count() == 4.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #3.
	 */
	
	 struct rb;
	 
	 zassert_ok(rb_init(4), "rb_init(4) should succeed");
	 zassert_ok(rb_push(1), "rb_push(1) should succeed");
	 zassert_ok(rb_push(2), "rb_push(2) should succeed");
	 zassert_ok(rb_push(3), "rb_push(3) should succeed");
	 zassert_ok(rb_push(4), "rb_push(4) should succeed");
	 zassert_true(rb_is_full(), "The buffer should be full after pushing 4 items");
	 zassert_equal(rb_count(), 4, "The buffer count should be 4 after pushing 4 items");
}
