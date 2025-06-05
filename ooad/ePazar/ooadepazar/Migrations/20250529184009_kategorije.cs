using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace ooadepazar.Migrations
{
    /// <inheritdoc />
    public partial class kategorije : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.AddColumn<int>(
                name: "Kategorija",
                table: "Artikal",
                type: "int",
                nullable: false,
                defaultValue: 0);

            migrationBuilder.AddColumn<string>(
                name: "SlikaUrl",
                table: "Artikal",
                type: "nvarchar(max)",
                nullable: true);
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropColumn(
                name: "Kategorija",
                table: "Artikal");

            migrationBuilder.DropColumn(
                name: "SlikaUrl",
                table: "Artikal");
        }
    }
}
