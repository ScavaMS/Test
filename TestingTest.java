package test;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import src.Testing;

public class TestingTest {
    @Test
    void testDoble() {
        Integer resultado = Testing.doble(4);
        Assertions.assertEquals(8, resultado);

    }

    @Test
    void testTriple() {
        
    }
}
