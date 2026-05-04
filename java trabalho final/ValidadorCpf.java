public class ValidadorCpf {

    // Método principal que valida um CPF
    public static boolean validarCPF(String cpf) {

        if (cpf == null) return false;

        // Remover pontos e hífen
        cpf = cpf.replace(".", "").replace("-", "").trim();

        // Deve ter exatamente 11 dígitos
        if (cpf.length() != 11) return false;

        // CPF não pode ter todos os dígitos iguais
        if (cpf.matches("(\\d)\\1{10}")) return false;

        // Calcula primeiro e segundo dígito
        int dv1 = calcularDigito(cpf, 10);
        int dv2 = calcularDigito(cpf, 11);

        // Verifica se os dígitos calculados são iguais aos informados
        return (dv1 == Character.getNumericValue(cpf.charAt(9)) &&
                dv2 == Character.getNumericValue(cpf.charAt(10)));
    }

    // Calcula DV1 ou DV2 conforme o peso inicial (10 ou 11)
    private static int calcularDigito(String cpf, int pesoInicial) {
        int soma = 0;
        int peso = pesoInicial;

        // DV1 usa os 9 primeiros dígitos
        // DV2 usa os 10 primeiros dígitos
        int limite = (pesoInicial == 10 ? 9 : 10);

        for (int i = 0; i < limite; i++) {
            int num = Character.getNumericValue(cpf.charAt(i));
            soma += num * peso;
            peso--;
        }

        int resto = soma % 11;

        return (resto < 2) ? 0 : (11 - resto);
    }
}