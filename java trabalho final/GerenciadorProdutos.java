import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * Gerencia produtos: CRUD, buscas, seleção para venda.
 * Recebe Scanner no construtor para ler do console.
 */
public class GerenciadorProdutos {
    private List<Produto> lista;
    private int proxId;
    private Scanner sc;

    public GerenciadorProdutos(Scanner sc) {
        this.lista = new ArrayList<Produto>();
        this.proxId = 1;
        this.sc = sc;
    }

    // menu interativo
    public void menu() {
        int op = -1;
        while (op != 0) {
            System.out.println("\n--- MÓDULO PRODUTOS ---");
            System.out.println("1 - Incluir Produto");
            System.out.println("2 - Excluir Produto");
            System.out.println("3 - Listar Produtos");
            System.out.println("4 - Atualizar Estoque");
            System.out.println("5 - Buscar Produto");
            System.out.println("0 - Voltar");
            System.out.print("Escolha: ");
            try { op = Integer.parseInt(sc.nextLine()); } catch (Exception e) { System.out.println("Entrada inválida."); continue; }

            switch (op) {
                case 1: incluir(); break;
                case 2: excluir(); break;
                case 3: listar(); break;
                case 4: atualizar(); break;
                case 5: buscar(); break;
                case 0: break;
                default: System.out.println("Opção inválida.");
            }
        }
    }

    // adiciona produto
    public void incluir() {
        System.out.print("Nome do produto: ");
        String nome = sc.nextLine().trim();
        if (nome.isEmpty()) { System.out.println("Nome vazio."); return; }
        if (existsByName(nome)) { System.out.println("Produto com esse nome já existe."); return; }

        System.out.print("Quantidade inicial (inteiro >=0): ");
        int qtd;
        try { qtd = Integer.parseInt(sc.nextLine()); if (qtd < 0) throw new Exception(); } catch (Exception e) { System.out.println("Quantidade inválida."); return; }

        System.out.print("Preço unitário (ex: 12.50): ");
        double preco;
        try { preco = Double.parseDouble(sc.nextLine()); if (preco < 0) throw new Exception(); } catch (Exception e) { System.out.println("Preço inválido."); return; }

        Produto p = new Produto(proxId++, nome, qtd, preco);
        lista.add(p);
        System.out.println("Produto cadastrado: " + p.brief());
    }

    // remove produto por id (só se não foi vendido)
    public void excluir() {
        System.out.print("ID do produto a excluir: ");
        int id;
        try { id = Integer.parseInt(sc.nextLine()); } catch (Exception e) { System.out.println("ID inválido."); return; }
        Produto p = buscarPorId(id);
        if (p == null) { System.out.println("Produto não encontrado."); return; }
        if (p.foiVendido()) { System.out.println("Não é permitido excluir produto que já foi vendido."); return; }
        lista.remove(p);
        System.out.println("Produto removido.");
    }

    // listar todos
    public void listar() {
        if (lista.isEmpty()) { System.out.println("Nenhum produto cadastrado."); return; }
        System.out.println("\n--- LISTA DE PRODUTOS ---");
        for (Produto p : lista) System.out.println(p.toString());
    }

    // atualizar estoque (adicionar/remover)
    public void atualizar() {
        System.out.print("ID do produto: ");
        int id;
        try { id = Integer.parseInt(sc.nextLine()); } catch (Exception e) { System.out.println("ID inválido."); return; }
        Produto p = buscarPorId(id);
        if (p == null) { System.out.println("Produto não encontrado."); return; }

        System.out.print("Digite quantidade (use negativo para remover, ex: -2): ");
        int delta;
        try { delta = Integer.parseInt(sc.nextLine()); } catch (Exception e) { System.out.println("Quantidade inválida."); return; }
        if (p.getQuantidade() + delta < 0) { System.out.println("Operação inválida: estoque negativo."); return; }
        p.setQuantidade(p.getQuantidade() + delta);
        System.out.println("Estoque atualizado: " + p.brief());
    }

    // busca por id ou parte do nome
    public void buscar() {
        System.out.print("Digite ID ou parte do nome: ");
        String termo = sc.nextLine().trim();
        try {
            int id = Integer.parseInt(termo);
            Produto p = buscarPorId(id);
            if (p != null) { System.out.println(p.toString()); return; }
        } catch (Exception e) { /* não era ID */ }

        boolean achou = false;
        for (Produto p : lista) {
            if (p.getNome().toLowerCase().contains(termo.toLowerCase())) {
                System.out.println(p.toString());
                achou = true;
            }
        }
        if (!achou) System.out.println("Nenhum produto encontrado.");
    }

    // seleciona produto para venda (retorna null se cancelar)
    public Produto selecionarProduto() {
        if (lista.isEmpty()) {
            System.out.println("Nenhum produto cadastrado.");
            return null;
        }
        System.out.println("\nSelecione produto por ID (ou 'C' para cancelar): ");
        listar();
        System.out.print("ID: ");
        String entrada = sc.nextLine().trim();
        if (entrada.equalsIgnoreCase("C")) return null;
        int id;
        try { id = Integer.parseInt(entrada); } catch (Exception e) { System.out.println("ID inválido."); return null; }
        Produto p = buscarPorId(id);
        if (p == null) { System.out.println("Produto não encontrado."); return null; }
        return p;
    }

    // checa existência por nome (case-insensitive)
    private boolean existsByName(String nome) {
        for (Produto p : lista) if (p.getNome().equalsIgnoreCase(nome)) return true;
        return false;
    }

    // busca por id
    private Produto buscarPorId(int id) {
        for (Produto p : lista) if (p.getId() == id) return p;
        return null;
    }

    // método usado por CadastroVendas para marcar produto vendido
    public void marcarProdutoVendido(Produto p) {
        if (p != null) p.marcarComoVendido();
    }
}
